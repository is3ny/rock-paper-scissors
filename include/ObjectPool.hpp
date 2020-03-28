#pragma once

#include <map>

/*! ObjectPool class is a generic class that manages a pool of objects.
*/
template<typename T, typename K>
class ObjectPool
{
public:
    ObjectPool() = default;

    void Add(const K& id, T&& obj)
    {
        m_pool[id] = obj;
    }

    /*!
        /throws domain_error if no object is assigned requested id.
    */
    T& Get(const K& id)
    {
        if (m_pool.find(id) == m_pool.end())
            throw std::domain_error("Request for inexistent object");
        return m_pool[id];
    }

    /*!
        /nothrow
    */
    void Remove(const K& id)
    {
        m_pool.erase(id);
    }

    void Clear() 
    {
        m_pool.clear();
    }

private:
    std::map<K, T> m_pool;
};