#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <class T>
class Singleton {
public:
  Singleton(Singleton const&) = delete;
  void operator=(Singleton const&) = delete;

  static T* getInstance() {
    return pInstance_;
  }

  static bool create() {
    if (!pInstance_) {
      pInstance_ = new T();
      return pInstance_ != nullptr;
    }
    else
      return false;
  }

  static bool destroy() {
    if (pInstance_) {
      delete pInstance_;
      pInstance_ = nullptr;

      return true;
    }
    else
      return false;
  }

protected:
  Singleton() {}
  ~Singleton() {}

private:
  static T* pInstance_;
};

template <class T>
T* Singleton<T>::pInstance_ = NULL;

#endif // __SINGLETON_H__
