/********************************
*   Clase Numero y Tarjeta      *
*   Fichero cabecera            *
*   Autor: FcoJ Rosa            *
********************************/

#ifndef TARJETA_HPP
#define TARJETA_HPP
#include <set>
#include "cadena.hpp"
#include "usuario.hpp"

class Clave;
class Usuario;


class Numero
{
public:
    Numero(const Cadena& numero);
    enum Razon {LONGITUD, DIGITOS, NO_VALIDO};
    class Incorrecto
    {
    public:
        Incorrecto(Razon motivo):motivo_(motivo){}
        Razon razon() const{return motivo_;}
    private:
        Razon motivo_;
    };
    operator const char*() const;
private:
    Cadena numero_;
};

class Tarjeta
{
public:
    enum Tipo {Otro, VISA, Mastercard, Maestro, JCB, AmericanExpress};
    Tarjeta(const Numero& numTarjeta, const Usuario& titular, const Fecha& caducidad);
    class Caducada
    {
    public:
        Caducada(const Fecha& caducada):caducada_(caducada){}
        const Fecha& cuando() const{return caducada_;}
    private:
        Fecha caducada_;
    };
    class Num_duplicado
    {
    public:
        Num_duplicado(const Numero& duplicado):duplicado_(duplicado){}
        const Numero& que() const{return duplicado_;}
    private:
        Numero duplicado_;
    };
    Tarjeta(const Tarjeta&) = delete; //c++11
    Tarjeta& operator=(const Tarjeta&) = delete; //c++11
    const Numero& numero() const{return numTarjeta_;}
    const Usuario* titular() const{return titular_;}
    const Fecha& caducidad() const {return caducidad_;}
    bool activa() const{return activa_;}
    enum Tipo tipo() const;
    bool activa(bool activar);
    class Desactivada
    {};
    void anula_titular();
    ~Tarjeta();
private:
    const Numero numTarjeta_;
    const Usuario* titular_;
    const Fecha caducidad_;
    bool activa_;
    static std::set<Numero> numsTarjetas_;
};

bool operator<(const Numero& n1, const Numero& n2);
std::ostream& operator<<(std::ostream& os, const Tarjeta& t);
bool operator<(const Tarjeta& t1, const Tarjeta& t2);


#endif //TARJETA_HPP
