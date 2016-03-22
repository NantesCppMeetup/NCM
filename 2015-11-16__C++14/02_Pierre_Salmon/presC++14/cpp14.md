class: center, middle

# C++11/C++14 - Le retour de la bête

---
# Historique

* 1998: ISO C++ adoptée (C++98)

* 2003: TC1 - "Technical Corrigendum 1" (C++03)

* 2005: TR1 - Libray "Technical Report 1"

* 2009: Choix des nouvelles futures (C++0X)

* 2011: ISO C++0x adoptée (C++11)

* 2013: Draft C++14 adoptée 

* 2014: révision mineur

* 2017: révision majeur

---
class: 
# La technologie C++ qu'est ce que c'est ?

* Le langage C++

* La bibliothèque standard

* L'ensemble des bibliothèques [boost](http://www.boost.org) 

---
class: center,middle
# C+11/C++14 - Nouveauté du langage
---
class: center, middle
# Move semantic
Ou comment redéfinir:
```cpp
a = b;
```
---
class:
# Move semantic - LValue/RValue
Exemple de **LValue**:
```cpp
int i = 42;
i = 43;                 // i est une lvalue
int& p = &i;            // p est une lvalue
int& result();
result() = 43;          // result() est une lvalue
int* pi = &result();    // pi est une lvalue
```
Exemple de **RValue**:
```cpp
int result();
int j = 0;
j = result();           // result() est une rvalue
int* pj = &result();    // Erreur
j = 42;                 // 42 est une rvalue
j = i * j;              // i * j est une rvalue
```
---
# Move semantic - LValue/RValue
Comment spécifier la fonction à utiliser en fonction d'une lvalue ou rvalue ?

```cpp
class X;

X i;
X result();

void foo(X& x);       // Attend une lvalue
void foo(X&& x);      // Attend une rvalue

foo(i);	              // Appel de foo(X& x)
foo(result());        // Appel de foo(X&& x)

foo(std::move(i));    // Appel de foo(X&& x)
```
---
# Move semantic - sample
```cpp
class buffer {
public;
  buffer();

  buffer(std::size_t l);
  
  ~buffer();
  
private:
  std::size_t 	len_;
  uint8_t* 	data_{nullptr};
};
```
---
# Move semantic - copy vs move 

```cpp
class buffer {
public;
  buffer();

  buffer(std::size_t l);
  buffer(buffer && buf);
  buffer(const buffer& buf);
  
  ~buffer();
  
private:
  std::size_t 	len_;
  uint8_t* 	data_{nullptr};
};

buffer b1{10000}, b2{1000000};

b1 = b2;
b1 = std::move(b2);

```

---
# Move semantic

```cpp
class toto {
public:
  toto();
  
  // DTOR
  ~toto();
  
  // CTOR
  toto(const toto& );
  toto(toto&& );
  
  // operator=
  toto& operator= (const toto& );
  toto& operator= (toto&& );
};
```
---
# Move semantic - perfect forwarding

Première version:
```cpp
template<typename T, typename Arg>
shared_ptr<T> factory(Args arg)
{
  return shared_ptr<T>(new T{arg});
}
```

Seconde version:
```cpp
template<typename T, typename Arg>
shared_ptr<T> factory(Args& arg)
{
  return shared_ptr<T>(new T{arg});
}

```
---
# Move semantic - perfect forwarding

Mais il y a quelques problèmes:

```cpp
factory(42);     // Erreur
factory(foo());  // Erreur
```
Troisième version:
```cpp
template<typename T, typename Arg>
shared_ptr<T> factory(const Args& arg)
{
  return shared_ptr<T>(new T{arg});
}
```
Mais ce n'est pas encore ça...

---
# Move semantic - perfect forwarding

Solution:
```cpp
template<typename T, typename Arg>
shared_ptr<T> factory(Arg&& arg)
{
  return shared_ptr<T>(new T{std::forward<Arg>(arg)});
}
```

Solution encore meilleurs (oui c'est possible) :
```cpp
template<typename T, typename ...Args>
shared_ptr<T> factory(Arg&& ...args)
{
  return shared_ptr<T>(new T{std::forward<Args>(args)...});
}
```
---
class: center,middle
# Mot clés contextuels
---
class:
# Mot clés contextuels
### final
```cpp
struct toto final {
...
};
```
### override
```cpp
struct A {
  virtual foo();
};

struct B : public A {
  foo() override;
  toto() override;        // Erreur
  foo() const override;   // Erreur
};
```
---
# Mot clés contextuels
### noexcept
```cpp
struct A {
  void foo() noexcept;
};
```
### delete/default
```cpp
struct A {
  A() = delete;
  A(const A& a) = default;
  
  template<typename T>
  void foo(T v) { ... }
  
  foo(int v) = delete;
};
```

---
# auto / decltype
* **auto** : le Graal...
* **decltype** : pour ne pas se sentir seul

```cpp
std::map<std::string, toto>   map_id_to_toto;

auto found = map_id_to_toto.find("titi");

const auto& value = found->second;

decltype(found) it = map_id_to_toto.begin();

decltype(found->second) value;
```

---
# Constexpr function
Enfin une généralisation des expressions constantes

```cpp

constexpr int amazing_func(int a, int b)
{ return a * b; }

const int mean_of_life = amazing_func(6, 7);

struct A {
  constexpr float pi = 3.1416f;
};

std::array<amazing_func(6,7), int>   array_int;

```

---
# Range based loop

Une boucle à la "java"...

Applicable sur tout les containers ayant le concept d'itérateur implémenté
```cpp
std::vector<int>  data;

for (int i : data) { ... } 
```

On peut faire beaucoup mieux

```cpp
std::map<std::string, int> data;

for (const auto& d : data) { ... }

```

Merci **auto**

---
# La notion de null

Uniformisation de la notion de null (ce n'est plus égal à (void*)0)

```cpp

int* p = nullptr;

int v = nullptr       // Erreur

if (p == nullptr) {}  // Fine !!

```
Enfin un vrai type nullptr...

Pour la compatibilité

```cpp
int* p = nullptr;

if (p == NULL) {}     // Fine !!

```

---
class: center,middle
# Les enums avancées

---
class:
# Les enums avancées

Améliorons nos enums
```cpp

enum Color { Red = 1, Green = 2, Blue = 3 };

int value = Red;     // Un int... en est-on sure ?

```

Si on définissait un type de base
```cpp
enum Color : std::uint8_t { Red = 1, Green = 2, Blue = 3 };

std::underlying_type<Color>::type value = Red;
```
---
# Les enums avancées

Maintenant, considérons les enums comme des classes à part entière

```cpp
enum class Elevation { Low, High };
enum class Color : int { Red, Green, Blue };

Color value = Red;               // Error
Color value = Color::Red;        // Fine !!
int bad_value = Color::Red;      // Error

```

---
class: center,middle
# Gestion de l'unicode
[Excellente réponse sur StackOverflow](http://stackoverflow.com/questions/17103925/how-well-is-unicode-supported-in-c11)
---
class:
# Raw string Literal
Enfin un vrai mode *verbatim*

```cpp
std::string value(R"(\n\n)");    // La chaine de caractère est \n\n

std::string with_new_line(R"(Le sanglot long
des violons ne pourront
rien changer)");
```
Les délimiteurs peuvent être modifiés
```cpp
std::string value(R"aabb(toto va au cirque)aabb");  // Affiche toto va au cirque
```

---
# Delegate/Inhirate constructor
```cpp
struct base {
  base()
  : base(0) 
  {}
  
  base(int i)
  : base(i, 0)
  {}
  
  base(int i, intj);
};

struct derived : base {
  using base::base;
};

derived d;
derived d1{10};

```
---
class: center,middle
# Syntaxe d'initialisation
---
class:
# Syntaxe d'initialisation

Gros travail d'uniformisation
```cpp
const int val1{10};

int a[] {1, 2, val1};
const std::vector<int> vec{ 1, 2 a[0]};

struct Point2D {};
const Point2D p1 = { 0.5f, 1.0f };

const float* data = new const float[4] { 0.5f, 1.2f, 3.1416f, 12f };
```
Même plus fort
```cpp
Point2D makeRootPoint() { return { 0.0f, 0.0f };}

void foo(const std::vector<int>& v);
foo({ 1, 2, 3});
```
---
# Syntaxe d'initialisation

Il y a 2 types d'initialisation :

## Aggregate

Initialisation de tableaux ou d'élements en suivant un ordre
```cpp
std::vector<int> v{ 1, 2, 3};
```

## Non aggregate

Initialisation en appelant un constructeur
```cpp
std::string val{"le sanglot long des violons"};
```
---
# Syntaxe d'initialisation
## Aggregate
Pour un UDT (User Defined Type)
```cpp
struct Point2D { int x; int y; };
Point2D p1 = { 10 };      // égal à { 10, 0 }
Point2D p2 = { 1, 2, 3};  // Error
```
## Non Aggregate
```cpp
struct Point2D {
  Point2D(int x, int y);
};

Point2D p1{0, 1}     // Fine !! équivalent à p1(0,1)
Point2D p2{0};       // Erreur
Point2D p3{0,1,2};   // Erreur
```
---
class: center, middle
# Attention: pas de conversion implicite de type

---
class:
# Syntaxe d'initialisation - initializer list
```cpp
#include <initializer_list>

class widget {
public:
  widget(std::initializer_list<int> nameIds)
  {
    std::cout << "size; " << nameIds.size();
    for (auto s : nameIds) {
      std::cout << "value: " << s << std::endl;
    }
  }
};
```
La liste d'initialisation possède 3 fonctions :
* size
* begin
* end

---
# Lambda expression ou les fonctions anonymes

Un petit exemple pour commencer :
```cpp
std::vector<int>  v;

auto it = std::find_if(v.cbegin(), v.cend(),
  [](int i) { return i % 2 == 0;  }
);
```
On génère le pseudo code suivant :
```cpp
class magicType {
public:
  bool operator() (int i) { return i % 2 == 0; }
};

auto it = std::find_if(v.cbegin(), v.cend(),
  magicType()
);
```
---
# Lambda expression - Capture
Comment capturer les variables locales
```cpp
int a,b;
auto f1 = [a,b](){ return a; };   // Copy de la variable a et b
auto f2 = [&a]() { return a; };   // Référence vers la variable a
auto f3 = [=]() { return a; };    // Copy des variables locales du scope
```
Dans une méthode membre d'une classe
```cpp
struct A {
  void print();
  void foo() {
    auto f1 = [&]() { this->print(); };
    f1();
  }
};
```
**Attention** - faire très attention aux cycle de vie des variables capturées

---
# Lambda expression - Closure
```cpp
using closure_type = std::function<void(int i)>;

closure_type factory_make(int id)
{
  return [id](int value) { return id << 32 + value; };
}

auto f = factory_make(23);
auto id = f(10);
```
---
# Lambda expression - C++14
### Auto déduction des types de paramètres et des valeurs de retour
```cpp
auto f1 = [](const auto& v1, const auto& v2) { return v1 > v2; };
```
### Clause de capture complexe avec move semantic
```cpp
auto f1 = [](int i){ return i * 2; };

auto f2 = [cb=std::move(f1)](int j) {
  return cb(j) + j;
};

```

---
# Alias Template
Pour ceux qui se rappelle du petit "bug"

```cpp
std::list<std::list>>  list_type;    // C++03 compilation Error - OK en C++14/14
```
Le mots clé using est utilisé pour créé des alias
```cpp
using list_type = std::list<int>;

template<typename T>
using list_shared_type = std::list<std::shared_ptr<T>>;
```

**Attention** : un alias n'est pas l'équivalent d'un typedef.

---
# Variadic template
Vive la métaprogrammation !!!

```cpp
template<typename Tuple, typename Func, std::size_t ...Indices>
void for_each_impl(Tuple&& tuple, Func&& func, std::index_sequence<Indices...>)
{
  using t_type int[];
  (void)t_type{1,
    (func(std::get<Indices>(std::forward<Tuple>())), void(), int{})...
  };
}

template<typename Tuple, typename Func>
void for_each(Tuple&& tuple, Func&& func)
{
  constexpr size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
  
  for_each_impl(std::forward<Tuple>(tuple), std::forward<Func>(func),
    std::make_index_sequence<N>{}
  );
}

for_each(std::make_tuple(1.0, "hello", 3), 
  [](auto v){ std::cout << v << std::endl;}
);

```

---
# Autres composants

C++11:
* User defined literal: 10_km, 30_sec, ...etc
* POD (Plain Object Data) definition
* & et && member functions

C++14:
* Binary Literal (C++14)
* Standard used defined literal: "Hello"s, 100ns, ...etc

---
# Biblio
* Effective Modern C++ - Scott Meyers
* Programming languages - C++ (Document 14882:2011,ISO/IEC)
* [cppreference](cppreference.com)
* Overview of The New C++ (C++11/14) - Scott Meyers

---
class: center,middle
# Questions ?

---
class: center,middle
# Petit plus : MPXX
[https://github.com/chybz/mpxx.git](https://github.com/chybz/mpxx.git)
