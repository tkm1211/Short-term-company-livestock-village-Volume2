#ifndef _SINGLETON_H_
#define _SINGLETON_H_

///<summary>
///Singletonパターン
///<para>インスタンスが一つしか生成されないことを保証するデザインパターン</para>
///</summary>
template<class T>
class Singleton
{
public:
    static T* getInstance()
    {
        static T instance;
        return &instance;
    }
};

#endif //_SINGLETON_H_
