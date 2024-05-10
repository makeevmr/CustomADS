#ifndef MIN_MAX_QUEUE_EXCEPTION_THROWING_CLASS
#define MIN_MAX_QUEUE_EXCEPTION_THROWING_CLASS
#include <exception>
#include <iostream>

class ExcThrowClass {
private:
    static unsigned int created_objects;
    int *data;

public:
    ExcThrowClass() {
        ++created_objects;
        if (created_objects % 20 == 0) {
            throw std::runtime_error("Exception in default constructor");
        }
        data = new int[10];
        for (int i = 0; i < 10; ++i) {
            data[i] = i;
        }
    }

    ExcThrowClass(const ExcThrowClass &other) {
        ++created_objects;
        if (created_objects % 20 == 0) {
            throw std::runtime_error("Exception in copy constructor");
        }
        data = new int[10];
        for (int i = 0; i < 10; ++i) {
            data[i] = other.data[i];
        }
    }

    ExcThrowClass &operator=(const ExcThrowClass &other) {
        if (this != &other) {
            delete[] data;
            data = new int[10];
            for (int i = 0; i < 10; ++i) {
                data[i] = i;
            }
        }
        return *this;
    }

    ExcThrowClass(ExcThrowClass &&other) noexcept : data(other.data) {
        ++created_objects;
        other.data = nullptr;
    }

    ExcThrowClass &operator=(ExcThrowClass &&other) noexcept {
        ++created_objects;
        if (this != &other) {
            delete[] data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    ~ExcThrowClass() {
        --created_objects;
        delete[] data;
    }
};

unsigned int ExcThrowClass::created_objects = 0;

#endif // MIN_MAX_QUEUE_EXCEPTION_THROWING_CLASS
