#ifndef _SINGLETON_H_
#define _SINGLETON_H_

///<summary>
///Singleton�p�^�[��
///<para>�C���X�^���X���������������Ȃ����Ƃ�ۏ؂���f�U�C���p�^�[��</para>
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
