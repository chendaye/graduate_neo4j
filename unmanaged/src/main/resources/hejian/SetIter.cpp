// 文件名：SetIter.cpp
// 作用：从总数为max的集合（只包含一个相同元素）中，按要求枚举出有size个元素的子集合。使用stl

#include "SetIter.h"
#include <math.h>
#include <assert.h>

// 构造函数
// 输入参数：max表示集合元素的个数
CSetIter::CSetIter(unsigned long max) :
    m_ele_num(max,1)
{
    m_max = max;
}


// 析构函数
CSetIter::~CSetIter()
{
}

// 初始化枚举
// 输入参数：size表示子集合元素个数
// 输入参数：type表示枚举类型
void CSetIter::Init(unsigned long size, CSetIter::IterType type)
{
    m_size = size;
    m_iter_num = 0;
    m_iter_max = 0;
    switch (type)
    {
    case MULT_ORDERED:
        m_iter_max = 1;
        for (int i=0; i<size; i++)
            m_iter_max *= m_max;
        m_pfnIsSubsetOk = &CSetIter::IsMultOrdered;
        break;
    case MULT_DISORDER:
        m_iter_max = 1;
        for (int i=0; i<size; i++)
            m_iter_max *= m_max;

        m_pfnIsSubsetOk = &CSetIter::IsMultDisorder;
        break;

    case SINGLE_ORDERED:
        if (size > m_max)    // 此条件下子集个数为0
            break;

        for (int i=1; i<size; i++)
            m_iter_num = m_iter_num*m_max+i;

        for (int i=0; i<size; i++)
            m_iter_max = m_iter_max*m_max+(m_max-i-1);
        m_iter_max++;
        m_pfnIsSubsetOk = &CSetIter::IsSingleOrdered;
        break;
    case SINGLE_DISORDER:
        if (size > m_max)    // 此条件下子集个数为0
            break;

        for (int i=1; i<size; i++)
            m_iter_num = m_iter_num*m_max+i;

        for (int i=0; i<size; i++)
            m_iter_max = m_iter_max*m_max+(m_max-size+i);
        m_iter_max++;
        
        m_pfnIsSubsetOk = &CSetIter::IsSingleDisorder;
        break;

    case MULT_DISORDER_IN:
        m_iter_max = 1;
        for (int i=0; i<size; i++)
            m_iter_max *= m_max;
        m_pfnIsSubsetOk = &CSetIter::IsMultDisorderIn;
        break;

    case MULT_ORDERED_IN:
        m_iter_max = 1;
        for (int i=0; i<size; i++)
            m_iter_max *= m_max;
        m_pfnIsSubsetOk = &CSetIter::IsMultOrderedIn;
        break;
    default:
        assert(!"Never come here");
    }
    m_iter_v.assign(m_size, 0);
    ConvertNumToVector();
}

// 得到下一个子集合
// 输出参数：subset得到的子集合
// 返回值：1表示成功取得，0表示没有取得，枚举完毕
int CSetIter::GetNextSubset(std::vector<int>& subset)
{

    assert(subset.size() == m_size);
    while (m_iter_num < m_iter_max)
    {
        // 判断是否符合条件
        if ((this->*m_pfnIsSubsetOk)(m_iter_v))
        {
            subset = m_iter_v;
            IncIterNum();
            return 1;
        }
        IncIterNum();
    }
    return 0;
}

// 子集合是否满足不重复，有序条件
// 输出参数：subset得到的子集合
// 返回值：1表示符合，0表示不符合
int CSetIter::IsSingleOrdered(std::vector<int>& subset)
{
    for (int i=0; i<m_size-1; i++)
    {
        for (int j=i+1; j<m_size; j++)
        {
            if (subset[i] == subset[j])
                return 0;
        }
    }
    return 1;
}

// 子集合是否满足可重复，有序条件
// 输出参数：subset得到的子集合
// 返回值：1表示符合，0表示不符合
int CSetIter::IsMultOrdered(std::vector<int>& subset)
{
    return 1;
}

// 子集合是否满足可重复，无序条件
// 输出参数：subset得到的子集合
// 返回值：1表示符合，0表示不符合
int CSetIter::IsMultDisorder(std::vector<int>& subset)
{
    for (int i=0; i<m_size-1; i++)
    {
        if (subset[i] > subset[i+1])
            return 0;
    }
    return 1;
}

// 子集合是否满足不重复，无序条件
// 输出参数：subset得到的子集合
// 返回值：1表示符合，0表示不符
int CSetIter::IsSingleDisorder(std::vector<int>& subset)
{
    for (int i=0; i<m_size-1; i++)
    {
        if (subset[i] >= subset[i+1])
            return 0;
    }
    return 1;
}


// 子集元素重复,无序，不能超出集合
// 输出参数：subset得到的子集合
// 返回值：1表示符合，0表示不符
int CSetIter::IsMultDisorderIn(std::vector<int>& subset)
{
    std::vector<int> tmp(m_ele_num.size(), 0);
    for (int i=0; i<m_size-1; i++)
    {
        if (subset[i] > subset[i+1])
            return 0;
    }
    for (int i=0; i<m_size; i++)
    {
        tmp[subset[i]]++;
        if (tmp[subset[i]] > m_ele_num[subset[i]])
            return 0;
    }
    return 1;
}

// 子集元素重复,有序，不能超出集合
// 输出参数：subset得到的子集合
// 返回值：1表示符合，0表示不符
int CSetIter::IsMultOrderedIn(std::vector<int>& subset)
{
    std::vector<int> tmp(m_ele_num.size(), 0);
    for (int i=0; i<m_size; i++)
    {
        tmp[subset[i]]++;
        if (tmp[subset[i]] > m_ele_num[subset[i]])
            return 0;
    }
    return 1;
}

// 迭代数增加
void CSetIter::IncIterNum()
{
    m_iter_num++;
    m_iter_v[m_iter_v.size()-1]++;
    for (int i=m_size-1; i>0; i--)
    {
        assert(m_iter_v[i] <= m_max);
        if (m_iter_v[i] == m_max)
        {
            m_iter_v[i] = 0;
            m_iter_v[i-1]++;
        }
        else
            break;
    }
}

// 把迭代数组变成迭代数
void CSetIter::ConvertNumToVector()
{
    unsigned long iter_tmp = m_iter_num;
    for (int i=m_size-1; i>=0; i--)
    {
        m_iter_v[i] = iter_tmp % m_max;
        iter_tmp /= m_max;
    }
}

// 把迭代数变成迭代数组
void CSetIter::ConvertVectorToNum()
{
    m_iter_num = 0;
    for (int i=0; i<m_size; i++)
    {
        m_iter_num = m_iter_num * m_max + m_iter_v[i];
    }
}
