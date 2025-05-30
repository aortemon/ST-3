// Copyright 2021 Uranus
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <thread>
#include <chrono>
#include "TimedDoor.h"

using ::testing::Return;

class MockTimerClient : public TimerClient {
 public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class MockDoor : public Door {
 public:
    MOCK_METHOD(void, lock, (), (override));
    MOCK_METHOD(void, unlock, (), (override));
    MOCK_METHOD(bool, isDoorOpened, (), (override));
};

TEST(TimedDoorTest, IsInitiallyClosed) {
    TimedDoor door(2);
    EXPECT_FALSE(door.isDoorOpened());
}

TEST(TimedDoorTest, CanBeOpened) {
    TimedDoor door(2);
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}

TEST(TimedDoorTest, CanBeClosedAfterOpening) {
    TimedDoor door(2);
    door.unlock();
    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST(TimerClientTest, TimeoutCalledAfterRegistration) {
    MockTimerClient mockClient;
    EXPECT_CALL(mockClient, Timeout()).Times(1);
    Timer timer;
    timer.tregister(2, &mockClient);
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

TEST(TimedDoorTest, ThrowsWhenOpenTooLong) {
    TimedDoor door(1);
    door.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    EXPECT_THROW(door.throwState(), std::runtime_error);
}

TEST(TimedDoorTest, DoesNotThrowWhenClosed) {
    TimedDoor door(1);
    door.unlock();
    door.lock();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    EXPECT_NO_THROW(door.throwState());
}

TEST(DoorTimerAdapterTest, AdapterThrowsWhenDoorOpenTooLong) {
    TimedDoor door(1);
    DoorTimerAdapter adapter(door);
    door.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    EXPECT_THROW(adapter.Timeout(), std::runtime_error);
}

TEST(TimerClientTest, TimeoutCalledImmediatelyAfterRegistration) {
    MockTimerClient mockClient;
    Timer timer;
    EXPECT_CALL(mockClient, Timeout()).Times(1);
    timer.tregister(1, &mockClient);
}

TEST(TimedDoorTest, DoesNotThrowWhenInitiallyClosed) {
    TimedDoor door(2);
    EXPECT_FALSE(door.isDoorOpened());
    EXPECT_NO_THROW(door.throwState());
}

TEST(DoorTimerAdapterTest, AdapterDoesNotThrowWhenDoorClosed) {
    MockDoor mockDoor;
    DoorTimerAdapter adapter((TimedDoor&)mockDoor);
    EXPECT_CALL(mockDoor, isDoorOpened()).WillOnce(Return(false));
    EXPECT_NO_THROW(adapter.Timeout());
}
