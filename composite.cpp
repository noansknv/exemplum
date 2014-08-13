#include <iostream>
#include <list>
#include <memory>

class BikeComponent {
public:
    virtual int weight() = 0;
};

class BikeCompositeComponent : public BikeComponent {
private:
    std::list<std::shared_ptr<BikeComponent>> _components;
public:
    BikeCompositeComponent() : _components() {}

    virtual void add(std::shared_ptr<BikeComponent> component) {
        this->_components.push_front(component);
    }

    virtual int weight() override {
        int weight = 0;

        for (auto it = this->_components.begin(); it != this->_components.end(); ++it) {
            weight += (*it)->weight();
        }

        return weight;
    }
};

class Wheel : public BikeCompositeComponent {
public:
    virtual int weight() override { return BikeCompositeComponent::weight() + 10; }
};

class Frame : public BikeCompositeComponent {
public:
    virtual int weight() override { return BikeCompositeComponent::weight() + 50; }
};

class DiscBrakes : public BikeComponent {
public:
    virtual int weight() override { return 6; }
};

class Disc : public BikeComponent {
public:
    virtual int weight() override { return 2; }
};



int main() {
    auto rearWheel = std::shared_ptr<Wheel>(new Wheel());
    auto frontWheel = std::shared_ptr<Wheel>(new Wheel());
    auto frame = std::shared_ptr<Frame>(new Frame());
    auto discBrakes = std::shared_ptr<DiscBrakes>(new DiscBrakes());
    auto rearDisc = std::shared_ptr<Disc>(new Disc());
    auto frontDisc = std::shared_ptr<Disc>(new Disc());

    rearWheel->add(rearDisc);
    frontWheel->add(frontDisc);
    frame->add(rearWheel);
    frame->add(frontWheel);
    frame->add(discBrakes);

    std::cout << "Bike weight = " << frame->weight() << std::endl;
}
