/********************************
*   Clase Clave y Usuario       *
*   Fichero cabecera            *
*   Autor: FcoJ Rosa            *
********************************/

#ifndef USUARIO_HPP
#define USUARIO_HPP
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "cadena.hpp"
#include "articulo.hpp"
#include "tarjeta.hpp"

class Numero;
class Tarjeta;


class Clave
{
public:
    Clave(const char* contrasenna);
    enum Razon {CORTA, ERROR_CRYPT};
    class Incorrecta
    {
    public:
        Incorrecta(Razon motivo):motivo_(motivo){}
        Razon razon() const{return motivo_;}
    private:
        Razon motivo_;
    };
    const Cadena& clave() const{return contrasenna_;}
    bool verifica(const char* contrasenna) const;
private:
    Cadena contrasenna_;
};

class Usuario
{
public:
    typedef std::map<Numero, Tarjeta*> Tarjetas;
    typedef std::unordered_map<Articulo*, unsigned> Articulos;
    Usuario(const Cadena& identificador, const Cadena& nombre, const Cadena& apellidos,
            const Cadena& direccion, const Clave& clave);
    class Id_duplicado
    {
    public:
        Id_duplicado(const Cadena& id):id_(id){}
        const Cadena& idd() const{return id_;}
    private:
        const Cadena id_;
    };
    Usuario(const Usuario&) = delete; //c++11
    Usuario& operator=(const Usuario&) = delete; //c++11
    const Cadena& id() const{return identificador_;}
    const Cadena& nombre() const{return nombre_;}
    const Cadena& apellidos() const{return apellidos_;}
    const Cadena& direccion() const{return direccionUsuario_;}
    const Tarjetas& tarjetas() const{return tarjetas_;}
    const Articulos& compra() const{return articulos_;}
    void es_titular_de(const Tarjeta& t);
    void no_es_titular_de(const Tarjeta& t);
    ~Usuario();
    void compra(const Articulo& a, unsigned cantidad=1);
    unsigned n_articulos() const;
    friend std::ostream& operator<<(std::ostream&, const Usuario&);
private:
    const Cadena identificador_;
    const Cadena nombre_;
    const Cadena apellidos_;
    const Cadena direccionUsuario_;
    Clave contrasenna_;
    Tarjetas tarjetas_;
    Articulos articulos_;
    static std::unordered_set<Cadena> identificadores_;
};

std::ostream& operator<<(std::ostream& os, const Usuario& u);
std::ostream& mostrar_carro(std::ostream& os, const Usuario& u);


#endif //USUARIO_HPP
