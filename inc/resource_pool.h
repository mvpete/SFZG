#ifndef __RESOURCE_H__
#define __RESOURCE_H__

class ResourcePtr
{
  class placeholder
  {
  public:
    virtual ~placeholder() { }
    virtual const std::type_info& type_info()=0;
  };

  template <typename T>
  struct impl : public placeholder
  {
    const T* _held;
  public:
    impl(const T *val):_held(val){}
    virtual ~impl() { delete _held; }
    virtual const std::type_info& type_info()
    {
      return typeid(*_held);
    }
  };

  placeholder *_value;
 public:
 ResourcePtr() :_value(0) {}
  ~ResourcePtr() { delete _value; }
  template <typename T>
  ResourcePtr(const T& val);

  const std::type_info& type_info() { return _value->type_info(); }

  template <typename value_type>
    value_type* ptr();
};

template <typename value_type>
value_type GetResourceValue(ResourcePtr &r)
{
  const value_type *value = r.ptr<value_type>();
  if(value)
    return *value;
  else
    throw std::bad_cast();
}


template <typename value_type>
ResourcePtr::ResourcePtr(const value_type &val) : _value(new impl<value_type>(val))
{
}


template <typename value_type>
value_type* ResourcePtr::ptr() 
{
  if( typeid(value_type) == type_info() )
    return static_cast<impl<value_type>>(_value)->_held;
  else
    return NULL;
}


class ResourcePool
{
  std::vector<ResourcePtr> _resources;
  
  public:

  ResourcePool();
  template<typename value_type>
  void AddResource(value_type *resource);


};


#endif
