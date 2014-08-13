#ifndef __EXEMPLUM_ABSTRACTFACTORY__
#define __EXEMPLUM_ABSTRACTFACTORY__

class IngredientFactory {
public:
  static IngredientFactory* Instance();
protected:
  IngredientFactory();
private:
  static IngredientFactory* _instance = null;
}
// Pattern: Phoenix-Singleton

/**
 * Phoenix-Singleton, where once all instances go out of scope, singleton gets destroyed.
 * Later, when singleton is queried again, new singleton instance is created.
 */
IngredientFactory* IngredientFactory::Instance() {
  // http://stackoverflow.com/questions/19147588/shared-pointers-to-a-singleton-do-not-recognize-each-other
  static std::weak_ptr<IngredientFactory> instance;
  static std::mutex mutex;
  const std::lock_guard<std::mutex> lock(mutex);
  if(const auto result = instance.lock()){
    return result;
  }

  return (instance = std::make_shared<Foo>()).lock();
}

#endif
