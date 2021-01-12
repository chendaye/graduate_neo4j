// 文件名：SetIter.h
// 作用：从总数为max的集合（只包含一个相同元素）中，按要求枚举出有size个元素的子集合。
#ifndef _SETITER_H_2007_09_02_
#define _SETITER_H_2007_09_02_

#include <vector>

class CSetIter
{
public:
    CSetIter(unsigned long max);
    ~CSetIter();
public:
    // 枚举方式
    enum IterType
    {
        MULT_ORDERED,        // 子集合元素可以重复，有序
        SINGLE_ORDERED,        // 子集元素不重复,有序
        MULT_DISORDER,        // 子集合元素可以重复，无序
        SINGLE_DISORDER,    // 子集元素不重复,无序
        MULT_DISORDER_IN,    // 子集元素重复,无序，不能超出集合
        MULT_ORDERED_IN        // 子集元素重复,有序，不能超出集合
    };
    // 枚举初始化
    void Init(unsigned long size, CSetIter::IterType type);
    // 得到下一个子集合
    int GetNextSubset(std::vector<int>& subset);
private:
    // 子集合是否满足可以重复，有序条件
    int IsMultOrdered(std::vector<int>& subset);
    // 子集合是否满足可以重复，无序条件
    int IsMultDisorder(std::vector<int>& subset);
    // 子集合是否满足不重复，有序条件
    int IsSingleOrdered(std::vector<int>& subset);
    // 子集合是否满足不重复，无序条件
    int IsSingleDisorder(std::vector<int>& subset);
    // 子集元素重复,无序，不能超出集合
    int IsMultDisorderIn(std::vector<int>& subset);
    // 子集元素重复,有序，不能超出集合
    int IsMultOrderedIn(std::vector<int>& subset);
    // 迭代数增加
    void IncIterNum();
    // 迭代数从数字到数组的转换
    void ConvertNumToVector();
    // 迭代数从数组到数字的转换
    void ConvertVectorToNum();

private:
    unsigned long m_max;            // 集合元素个数
    unsigned long m_size;            // 子集合元素个数
    unsigned long m_iter_max;        // 最大的迭代数
    unsigned long m_iter_num;        // 迭代数，用于遍历所有可能的子集合
    int (CSetIter::* m_pfnIsSubsetOk)(std::vector<int>& subset);    // 判断子集合是否满足要求
    std::vector<int> m_iter_v;
    std::vector<int> m_ele_num;
};


// Class CSetIterAgent
// 把一个其它类型的集合映射成正数集合，使更方便调用CSetIteI

// VC2005无法通过编译的
enum CSetIterAgent_SetType
{
    CSetIterAgent_MULT,    // 重集，可能包含相同元素
    CSetIterAgent_SINGLE    // 不包含相同元素
};
 
template<class Type>
class CSetIterAgent
{
    
//public:
//    // 集合类型
//    enum SetType
//    {
//        MULT,        // 重集，可能包含相同元素
//        SINGLE        // 不包含相同元素
//    };
public:
//    CSetIterAgent(const std::vector<Type>& set,CSetIterAgent::SetType type=CSetIterAgent::MULT);
    CSetIterAgent(const std::vector<Type>& set,CSetIterAgent_SetType type=CSetIterAgent_MULT);
    ~CSetIterAgent();
public:
    // 初始化
    void Init(unsigned long size, CSetIter::IterType type);
    // 得到下一个子集
    int GetNextSubset(std::vector<int> &subset);
    // 得到下一个子集
    int GetNextSubset(std::vector<Type>& subset, const std::vector<Type>& set);
private:
    std::vector<int> m_ele_num;        // 每个元素的个数
    std::vector<int> m_ele_set;        // 不同元素在原先集合的位置
    CSetIter* m_pSetIter;            // 枚举器
    unsigned long m_size;            // 子集的大小
};

// 构造函数，完成其它元素的集合到整数集合的映射
// 输入参数：set被枚举的集合
// 输入参数：type表示集合是否包含重复元素
template <class Type>
CSetIterAgent<Type>::CSetIterAgent(const std::vector<Type> &set, CSetIterAgent_SetType type)
{
    if (type == CSetIterAgent_MULT)
    {
        for (int i=0; i<set.size(); i++)
        {
            for (int j=0; j<m_ele_set.size(); j++)
            {
                if (set[m_ele_set[j]] == set[i])
                {
                    m_ele_num[j]++;
                    goto _next;
                }
            }
            m_ele_num.push_back(1);
            m_ele_set.push_back(i);
_next:
            ;
        }
    }
    else
    {
        m_ele_num.assign(set.size(),1);
        m_ele_set.assign(set.size(),0);
        for (int i=1; i<set.size(); i++)
            m_ele_set[i] = i;
    }
//    m_pSetIter = new CSetIter(m_ele_num.size(), m_ele_num);

}

// 析构函数
template <class Type>
CSetIterAgent<Type>::~CSetIterAgent()
{
    delete m_pSetIter;
}

// 初始化
// 输入参数：size表示子集元素个数
// 输入参数：type 枚举类型
template <class Type>
void CSetIterAgent<Type>::Init(unsigned long size, CSetIter::IterType type)
{
    m_size = size;
    m_pSetIter->Init(size, type);
}

// 得到下一个子集
// 输出参数：subset表示子集元素在原先集合的位置。
// 返回值：0表示枚举完成，其它表示没有
template <class Type>
int CSetIterAgent<Type>::GetNextSubset(std::vector<int> &subset)
{

    std::vector<int> tmp(m_size);
    int ret = m_pSetIter->GetNextSubset(tmp);
    for (int i=0; i<m_size; i++)
    {
        subset[i] = m_ele_set[tmp[i]];
    }
    return ret;
}

// 得到下一个子集
// 输入参数：set原先的集合
// 输出参数：subset表示子集元素
// 返回值：0表示枚举完成，其它表示没有
template <class Type>
int CSetIterAgent<Type>::GetNextSubset(std::vector<Type>& subset, const std::vector<Type>& set)
{
    std::vector<int> tmp(m_size);
    int ret = m_pSetIter->GetNextSubset(tmp);
    for (int i=0; i<m_size; i++)
    {
        subset[i] = set[m_ele_set[tmp[i]]];
    }
    return ret;
}

#endif        // _SETITER_H_2007_09_02_
