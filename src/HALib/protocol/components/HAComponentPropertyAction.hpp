#pragma once

#include "HAComponentProperty.hpp"

namespace HALIB_NAMESPACE
{
    template <typename T>
    class HAComponentPropertyAction : public HAComponentProperty
    {
    public:
        HAComponentPropertyAction(HAComponentPropertyKey name, const T callback, const char *value);
        T getCallback();

    protected:
    private:
        T mCallback;
    };

    template <typename T>
    HAComponentPropertyAction<T>::HAComponentPropertyAction(HAComponentPropertyKey name, const T callback, const char *value = NULL) : HAComponentProperty(name, value)
    {
        mCallback = callback;
    }

    template <typename T>
    T HAComponentPropertyAction<T>::getCallback()
    {
        return mCallback;
    }
} // namespace HALIB_NAMESPACE