/*
 * Singleton.h
 *
 *  Created on: 24 thg 1, 2017
 *      Author: MyPC
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

template <typename T>
class Singleton {
	public:
		static	T*	instance()
			{
				if (m_instance == nullptr)
					m_instance = new T();
				return m_instance;
			}
		Singleton(){};
		~Singleton(){};
	protected:
		static	T*	m_instance;
};

template <typename T>
T* Singleton<T>::m_instance = nullptr;

#endif /* SINGLETON_H_ */
