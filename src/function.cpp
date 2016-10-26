#include "function.h"

using namespace std;
using namespace ac;

Function::Function(const DataSet &ds, const Series* domain, const Series* codomain) :
    dataset(ds),
    domain(domain),
    codomain(codomain)
{
    set();
}

Function::Function(Function&& f) :
    dataset(f.dataset),
    domain(f.domain),
    codomain(f.codomain)
{
    f.domain = nullptr;
    f.codomain = nullptr;
    set();
}

Function::Function(const Function& f) :
    dataset(f.dataset),
    domain(f.domain),
    codomain(f.codomain)
{
    set();
}

Function::~Function()
{
    unset();
}

void Function::setDomain(const Series* domain)
{
    this->domain = domain;
}

void Function::setCodomain(const Series* codomain)
{
    this->codomain = codomain;
}

const Series& Function::getDomain()
{
    return *domain;
}

const Series& Function::getCodomain()
{
    return *codomain;
}

void Function::set()
{

}

void Function::unset()
{

}
