#ifndef XCORE_SINGLETON_H
#define XCORE_SINGLETON_H

namespace xcore
{

/// Insures that only one instance of a class in instantiated
/// Will recreate the instance if it gets deleted
/// not thread safe
template <class T>
class Singleton
{
   public:
      static T& instance()
      {
         return *instancePtr();
      }

      static boost::shared_ptr<T> instancePtr()
      {
         static boost::shared_ptr<T> instance = boost::shared_ptr<T>();
         if (instance.get() == NULL) {
            instance = boost::shared_ptr<T>(new T());
         }
         return instance;
      }

   protected:
      Singleton() {}

   private:
      Singleton(Singleton& copy);
};

} // namespace

#endif
