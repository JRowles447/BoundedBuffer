// tests.cpp
#include "../src/BoundedBuffer.h"
#include "../src/ProducerConsumer.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>

//Test checking if bounded buffer is empty
TEST(PCTest, Test1) {
    BoundedBuffer *BB = new BoundedBuffer(5);
    EXPECT_TRUE(BB->isEmpty());

    delete BB;
}

//Test checking append() and remove() from buffer
TEST(PCTest, Test2){
    BoundedBuffer *BB = new BoundedBuffer(5);
    BB->append(0);
    ASSERT_EQ(0,BB->remove());

    delete BB;
}


//Test checking append() and remove() from buffer on larger inputs
TEST(PCTest, Test3){
    BoundedBuffer *BB = new BoundedBuffer(5);
    BB->append(0);
    ASSERT_EQ(0,BB->remove());
    BB->append(39);
    BB->append(9);
    BB->append(8);
    ASSERT_EQ(39,BB->remove());
    ASSERT_EQ(9,BB->remove());
    BB->append(3);
    BB->append(10);
    ASSERT_EQ(8,BB->remove());
    ASSERT_EQ(3,BB->remove());
    ASSERT_EQ(10,BB->remove());
    ASSERT_TRUE(BB->isEmpty());

    delete BB;
}

// Test circular stuff
TEST(PCTest, Test4){
    BoundedBuffer *BB = new BoundedBuffer(5);
    BB->append(1);
    BB->append(2);
    BB->append(3);
    BB->append(4);
    BB->append(5);
    ASSERT_EQ(1, BB->remove());
    BB->append(6);
    ASSERT_EQ(2, BB->remove());
    ASSERT_EQ(3, BB->remove());
    BB->append(7);
    BB->append(8);
    // BB->append(9); // should overwrite here

    delete BB;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
