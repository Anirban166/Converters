#include <string>
#include <vector>
#include <sstream>
#include <iostream>

struct HtmlBuilder;
struct HtmlElement
{
  std::string name, text;
  std::vector<HtmlElement> elements;
  const size_t indentationSize = 2;

  HtmlElement() {}
  HtmlElement(const std::string& name, const std::string& text): name(name), text(text) {}

  std::string str(int indent = 0) const
  {
    std::ostringstream oss;
    std::string i(indentationSize * indent, ' ');
    oss << i << "<" << name << ">" << std::endl;

    if(text.size() > 0)
    {
      oss << std::string(indentationSize * (indent + 1), ' ') << text << std::endl;
    }

    for(const auto& e : elements)
    {
      oss << e.str(indent + 1);
    }

    oss << i << "</" << name << ">" << std::endl;
    return oss.str();
  }

  static std::unique_ptr<HtmlBuilder> build(std::string rootName)
  {
    return std::make_unique<HtmlBuilder>(rootName);
  }
};

struct HtmlBuilder
{
  HtmlBuilder(std::string rootName)
  {
    root.name = rootName;
  }
  HtmlBuilder& addChild(std::string childName, std::string childText)
  {
    HtmlElement element{childName, childText};
    root.elements.emplace_back(element);
    return *this;
  }
  std::string str() 
  { 
    return root.str(); 
  }
  operator HtmlElement() const 
  {   
    return root; 
  }
  HtmlElement root;
};

int main()
{
  HtmlBuilder exampleBody{"body"};
  exampleBody.addChild("p", "Unreal Engine 5.2 highlights from GDC '23");
  exampleBody.addChild("ul", "MetaHuman Animator").addChild("ul", "Substrate Material, Procedural Assemblies").addChild("ul", "Verse PL (Currently for UEFN and running atop the Blueprints VM only)");
  std::cout << exampleBody.str();
  /* Output:
  <body>
    <p>
      Unreal Engine 5.2 highlights from GDC '23
    </p>
    <ul>
      MetaHuman Animator
    </ul>
    <ul>
      Substrate Material, Procedural Assemblies
    </ul>
    <ul>
      Verse PL (Currently for UEFN and running atop the Blueprints VM only)
    </ul>
  </body>
  */
}