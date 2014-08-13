#include <iostream>
#include <string>
#include <map>

using namespace std;

class GlyphContext {
private:
    int _next;
    map<int, string> _decoration;
    GlyphContext() : _next { 0 }, _decoration { map<int, string>() } {}

public:
    static GlyphContext* getInstance() {
        return new GlyphContext();
    }

    void setDecoration(string decoration, int from, int to) {
        for (int i = from; i < to; ++i) {
            this->_decoration[i] = decoration;
        }
    }

    string getCurrentDecoration() {
        return this->_decoration[this->_next];
    }

    void next() {
        ++(this->_next);
    }

    void first() {
        this->_next = 0;
    }
};

class Glyph {
private:
    static int _counter;
    char _char;
    Glyph(char val) : _char { val } {
        ++(Glyph::_counter);
    }

public:
    static Glyph* getInstance(char c) {
        return new Glyph(c);
    }

    void draw(GlyphContext* context) {
        cout << context->getCurrentDecoration() << this->_char << context->getCurrentDecoration();
    }
};

int Glyph::_counter = 0;

class GlyphFactory {
private:
    map<char, Glyph*> _glyphDict;
    GlyphFactory() : _glyphDict { map<char, Glyph*>() } {}

public:
    static GlyphFactory* getInstance() {
        return new GlyphFactory();
    }

    Glyph* get(char c) {
        if (this->_glyphDict.count(c) == 0) {
            this->_glyphDict[c] = Glyph::getInstance(c);
        }

        return this->_glyphDict[c];
    }
};

class Text {
private:
    string _text;
    GlyphContext* _context;

public:
    Text(string text) : _text { text }, _context { GlyphContext::getInstance() } {}

    void setDecoration(string decoration, int from, int to) {
        this->_context->setDecoration(decoration, from, to);
    }

    void draw() {
        auto factory = GlyphFactory::getInstance();

        this->_context->first();

        for (auto t : this->_text) {
            auto g = factory->get(t);
            g->draw(this->_context);
            this->_context->next();
        }
    }
};

int main() {
    auto t = Text("Hello lalamido!");
    t.setDecoration("*", 0, 5);
    t.setDecoration("!", 4, 10);
    t.draw();
}
