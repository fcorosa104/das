/*********************************************************************************
*   Clase Articulo, Autor, ArticuloAlmacenable, Libro, Cederron y LibroDigital   *
*   Fichero cabecera                                                             *
*   Autor: FcoJ Rosa                                                             *
*********************************************************************************/

#ifndef ARTICULO_HPP
#define ARTICULO_HPP
#include <set>
#include "fecha.hpp"
#include "cadena.hpp"

class Autor
{
public:
    Autor(const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion);
    const Cadena& nombre() const{return nombre_;}
    const Cadena& apellidos() const{return apellidos_;}
    const Cadena& direccion() const{return direccion_;}
private:
    const Cadena nombre_;
    const Cadena apellidos_;
    const Cadena direccion_;
};

class Articulo
{
public:
    typedef std::set<Autor*> Autores;
    virtual void impresion_especifica(std::ostream& os) const=0;
    Articulo(const Autores& autores, const Cadena& referencia, const Cadena& titulo,
             const Fecha& publicacion, double precio);
    const Cadena& referencia() const{return codReferencia_;}
    const Cadena& titulo() const{return titulo_;}
    const Fecha& f_publi() const{return publicacion_;}
    double precio() const{return precio_;}
    double& precio() {return precio_;}
    class Autores_vacios
    {};
    const Autores& autores() const{return autores_;} //para la impresion
    virtual ~Articulo(){};
private:
    Autores autores_;
    const Cadena codReferencia_;
    const Cadena titulo_;
    const Fecha publicacion_;
    double precio_;
};

std::ostream& operator<<(std::ostream& os, const Articulo& a);

class ArticuloAlmacenable: public Articulo
{
public:
    //No implementamos impresion_especifica aqui, por lo que convierte esta tmb en abstracta
    ArticuloAlmacenable(const Autores& autores, const Cadena& referencia, const Cadena& titulo,
                        const Fecha& publicacion, double precio, unsigned stock=0);
    unsigned stock() const{return numALaVenta_;}
    unsigned& stock() {return numALaVenta_;}
private:
    unsigned numALaVenta_;
};

class Libro: public ArticuloAlmacenable
{
public:
    void impresion_especifica(std::ostream& os) const;
    Libro(const Autores& autores, const Cadena& referencia, const Cadena& titulo,
          const Fecha& publicacion, double precio, unsigned nPaginas, unsigned stock=0);
    const unsigned n_pag() const{return nPaginas_;}
private:
    const unsigned nPaginas_;
};

class Cederron: public ArticuloAlmacenable
{
public:
    void impresion_especifica(std::ostream& os) const;
    Cederron(const Autores& autores, const Cadena& referencia, const Cadena& titulo,
          const Fecha& publicacion, double precio, unsigned tamannoMB, unsigned stock=0);
    const unsigned tam() const{return tamannoMB_;}
private:
    const unsigned tamannoMB_;
};

class LibroDigital: public Articulo
{
public:
    void impresion_especifica(std::ostream& os) const;
    LibroDigital(const Autores& autores, const Cadena& referencia, const Cadena& titulo,
                 const Fecha& publicacion, double precio, const Fecha& expiracion);
    const Fecha& f_expir() const{return expiracion_;}
private:
    const Fecha expiracion_;
};

#endif //ARTICULO_HPP
