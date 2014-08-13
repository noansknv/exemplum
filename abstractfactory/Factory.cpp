// #include "AbstractFactory.hpp"
#include <memory>
#include <string>
#include <iostream>

// Pattern: AbstractFactory, Prototype, Singleton
namespace Exemplum {
  // Abstractions for Prototype factory
  class Modificator {
  public:
    virtual std::shared_ptr<Modificator> Clone() = 0;
    virtual std::string Type() = 0;
  };

  // Abstract Products for Abstract Factory
  class Stringable {
  public:
    explicit operator std::string() { return this->_ingredientName; }
  protected:
    Stringable(std::string ingredientName) {
      this->_ingredientName = ingredientName;
    }
  private:
    std::string _ingredientName;
  };

  class Salty : public Stringable {
  public:
    Salty(std::string ingredientName = "Salty") : Stringable(ingredientName) {}
  };

  class Sweet : public Stringable {
  public:
    Sweet(std::string ingredientName = "Sweet") : Stringable(ingredientName) {}
  };

  // Concrete Products for Abstract Factory
  class Salmon : public Salty {
  public:
    Salmon(std::string ingredientName = "Salmon") : Salty(ingredientName) {}
  };

  class Salt : public Salty {
  public:
      Salt(std::shared_ptr<Modificator> modificator,
           std::string ingredientName = "Salt")
         : Salty(modificator->Type() + " " + ingredientName) {}
  };

  class Sugar : public Sweet {
  public:
      Sugar(std::shared_ptr<Modificator> modificator,
            std::string ingredientName = "Sugar")
        : Sweet(modificator->Type() + " " + ingredientName) {}
  };

  class Chicken : public Sweet {
  public:
      Chicken(std::string ingredientName = "Chicken") : Sweet(ingredientName) {}
  };

  // Abstract Factory itself
  class IngredientFactory {
  public:
    virtual std::shared_ptr<Salty> makeSalty() const = 0;
    virtual std::shared_ptr<Sweet> makeSweet() const = 0;
  };

  class GMO : public Modificator {
  public:
    static GMO& Instance() {
      static GMO instance;
      return instance;
    }

    virtual std::string Type() override {
      return "GMO";
    }

    virtual std::shared_ptr<Modificator> Clone() override {
      return std::shared_ptr<GMO>(new GMO());
    }
  private:
    GMO() {}
    GMO(GMO const&);
    void operator=(GMO const&);
  };

  class NonGMO : public Modificator {
  public:
      static NonGMO& Instance() {
        static NonGMO instance;
        return instance;
      }

    virtual std::string Type() override {
      return "NonGMO";
    }
      virtual std::shared_ptr<Modificator> Clone() override {
        return std::shared_ptr<NonGMO>(new NonGMO());
      }
    private:
      NonGMO() {}
      NonGMO(NonGMO const&);
      void operator=(NonGMO const&);
  };

  // Concrete Factories for Abstract Factory
  class SeasoningPrototypeIngredientFactory : public IngredientFactory {
  public:
    SeasoningPrototypeIngredientFactory(Modificator& modificator) : _modificator(modificator) {}

    std::shared_ptr<Salty> makeSalty() const {
      return std::shared_ptr<Salt>(new Salt((this->_modificator).Clone()));
    }

    std::shared_ptr<Sweet> makeSweet() const {
      return std::shared_ptr<Sugar>(new Sugar((this->_modificator).Clone()));
    }
  private:
    Modificator& _modificator;
  };

  class MeatIngredientFactory : public IngredientFactory {
  public:
    std::shared_ptr<Salty> makeSalty() const {
      return std::shared_ptr<Salmon>(new Salmon());
    }

    std::shared_ptr<Sweet> makeSweet() const {
      return std::shared_ptr<Chicken>(new Chicken());
    }
  };

  // Client of Abstract Factory
  class Chef {
  public:
    void cook(IngredientFactory* factory) {
      std::shared_ptr<Sweet> sweet = factory->makeSweet();
      std::shared_ptr<Salty> salty = factory->makeSalty();

      std::cout << static_cast<std::string>(*sweet) << " + " << static_cast<std::string>(*salty) << std::endl;
    }
  };
}

int main() {
  Exemplum::IngredientFactory* gmoFactory = new Exemplum::SeasoningPrototypeIngredientFactory(Exemplum::GMO::Instance());
  Exemplum::IngredientFactory* nonGmoFactory = new Exemplum::SeasoningPrototypeIngredientFactory(Exemplum::NonGMO::Instance());
  Exemplum::IngredientFactory* meatFactory = new Exemplum::MeatIngredientFactory();
  Exemplum::Chef chef;
  chef.cook(gmoFactory);
  chef.cook(nonGmoFactory);
  chef.cook(meatFactory);
}
