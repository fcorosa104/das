/********************************
*   Clase Numero y Tarjeta      *
*   Fichero implmnt.            *
*   Autor: FcoJ Rosa            *
********************************/

#include <cstddef>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include "tarjeta.hpp"
#include "fecha.hpp"
#include "cadena.hpp"
using namespace std;

bool luhn(const Cadena& numero);
set<Numero> Tarjeta::numsTarjetas_;

Numero::Numero(const Cadena& numero)
{
    Cadena tmp(numero);
    Cadena::iterator itr = remove_if(tmp.begin(), tmp.end(),
                                    [](char c){return isspace(c);});
    if(itr != tmp.end())
        tmp = tmp.substr(0,itr-tmp.begin());
    Cadena::const_iterator it = find_if(tmp.cbegin(), tmp.cend(),
                                    [](char c){return (c < '0' || c > '9');});
    if(it != tmp.cend()) throw Incorrecto(DIGITOS);
    if(tmp.length() < 13 || tmp.length() > 18) throw Incorrecto(LONGITUD);
    if(!luhn(tmp)) throw Incorrecto(NO_VALIDO);
    numero_ = tmp;
}

Numero::operator const char*() const
{
    return numero_.c_str();
}

bool operator<(const Numero& n1, const Numero& n2)
{
    return Cadena((const char*)n1)<Cadena((const char*)n2);
}

Tarjeta::Tarjeta(const Numero& numTarjeta, const Usuario& titular, const Fecha& caducidad):
    numTarjeta_(numTarjeta),
    titular_(&titular),
    caducidad_(caducidad),
    activa_(false)
{
    if(caducidad < Fecha()) throw Caducada(caducidad);
    //NO PUEDE HABER DOS DUPLICADAS, LANZAR THROW SI PASA
    if(!numsTarjetas_.insert(numTarjeta).second)
        throw Num_duplicado(numTarjeta);
    //ASOCIAR LLAMANDO A USUARIO::ES_TITULAR_DE
    const_cast<Usuario*>(titular_)->es_titular_de(*this);
    activa_ = true;
}

Tarjeta::Tipo Tarjeta::tipo() const
{
    Cadena numT((const char*)numero());
    Cadena::iterator it=numT.begin();
    if(*it == '3')
    {
        ++it;
        if(*it == '4' || *it == '7')
            return Tarjeta::AmericanExpress;
        else
            return Tarjeta::JCB;
    }
    else if(*it == '4')
        return Tarjeta::VISA;
    else if(*it == '5')
        return Tarjeta::Mastercard;
    else if(*it == '6')
        return Tarjeta::Maestro;
    else
        return Tarjeta::Otro;
}

bool Tarjeta::activa(bool activar)
{
    activa_ = activar;
    return activa_;
}

void Tarjeta::anula_titular()
{
    titular_ = nullptr;
    activa_ = false;
}

Tarjeta::~Tarjeta()
{
    numsTarjetas_.erase(numTarjeta_);
    //LLAMAR A USUARIO_NO_ES_TITULAR SI NO HA SIDO DESTRUIDO PREVIAMENTE (leer mejor)
    if(titular_ != nullptr)
        const_cast<Usuario*>(titular_)->no_es_titular_de(*this);
}

ostream& operator<<(ostream& os, const Tarjeta& t)
{
    //PONER LINEAS SI ME DA TIEMPO
    switch(t.tipo())
    {
        case Tarjeta::VISA: os << "VISA" << endl; break;
        case Tarjeta::Mastercard: os << "Mastercard" << endl; break;
        case Tarjeta::Maestro: os << "Maestro" << endl; break;
        case Tarjeta::JCB: os << "JCB" << endl; break;
        case Tarjeta::AmericanExpress: os << "American Express" << endl; break;
        default: os << "Otro" << endl;
    }
    os << t.numero() << endl;
    for(Cadena::const_iterator it=t.titular()->nombre().cbegin(); it!=t.titular()->nombre().cend(); ++it)
    {
        os << (char)toupper(*it);
    }
    os << " ";
    for(Cadena::const_iterator it=t.titular()->apellidos().cbegin(); it!=t.titular()->apellidos().cend(); ++it)
    {
        os << (char)toupper(*it);
    }
    os << endl;
    os << "Caduca: " << setw(2) << setfill('0')
        << t.caducidad().mes()%100 << "/" << t.caducidad().anno()%100 << endl;
    return os;
}

bool operator<(const Tarjeta& t1, const Tarjeta& t2)
{
    return strcmp((const char*)t2.numero(),(const char*)t1.numero())>0;
}
