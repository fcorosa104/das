/*********************************************************************************
*   Clase Articulo, Autor, ArticuloAlmacenable, Libro, Cederron y LibroDigital   *
*   Fichero implmnt.                                                             *
*   Autor: FcoJ Rosa                                                             *
*********************************************************************************/

#include <iomanip>
#include "articulo.hpp"
using namespace std;

Articulo::Articulo(const Autores& autores, const Cadena& referencia, const Cadena& titulo,
                   const Fecha& publicacion, double precio):
                       autores_(autores),
                       codReferencia_(referencia),
                       titulo_(titulo),
                       publicacion_(publicacion),
                       precio_(precio)
{
    if(autores_.empty())
        throw Autores_vacios();
}

ostream& operator<<(ostream& os, const Articulo& a)
{
    os << "[" << a.referencia() << "] " << "\"" << a.titulo() << "\", de ";
    auto i = a.autores().begin();
    while(i != --a.autores().end())
    {
        os << (*i)->apellidos() << ", ";
        ++i;
    }
    os << (*i)->apellidos() << ". ";
    os << a.f_publi().anno() << ". " << fixed << setprecision(2) << a.precio() << " €" << "\n\t";
    a.impresion_especifica(os);
    return os;
}

ArticuloAlmacenable::ArticuloAlmacenable(const Autores& autores, const Cadena& referencia, const Cadena& titulo,
                                         const Fecha& publicacion, double precio, unsigned stock):
                       Articulo(autores,referencia,titulo,publicacion,precio),
                       numALaVenta_(stock){}

Libro::Libro(const Autores& autores, const Cadena& referencia, const Cadena& titulo, const Fecha& publicacion,
             double precio, unsigned nPaginas, unsigned stock):
                ArticuloAlmacenable(autores,referencia,titulo,publicacion,precio,stock),
                nPaginas_(nPaginas){}

void Libro::impresion_especifica(ostream& os) const
{
    os << n_pag() << " págs., " << stock();
    if(stock() == 1)
        os << " unidad.";
    else
        os << " unidades.";
}

Cederron::Cederron(const Autores& autores, const Cadena& referencia, const Cadena& titulo, const Fecha& publicacion,
                   double precio, unsigned tamannoMB, unsigned stock):
                    ArticuloAlmacenable(autores,referencia,titulo,publicacion,precio,stock),
                    tamannoMB_(tamannoMB){}

void Cederron::impresion_especifica(ostream& os) const
{
    os << tam() << " MB, " << stock();
    if(stock() == 1)
        os << " unidad.";
    else
        os << " unidades.";
}

LibroDigital::LibroDigital(const Autores& autores, const Cadena& referencia, const Cadena& titulo, const Fecha& publicacion,
                           double precio, const Fecha& expiracion):
                       Articulo(autores,referencia,titulo,publicacion,precio),
                       expiracion_(expiracion){}

void LibroDigital::impresion_especifica(ostream& os) const
{
    os << "A la venta hasta el " << expiracion_ << ".";
}

Autor::Autor(const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion):
                nombre_(nombre),
                apellidos_(apellidos),
                direccion_(direccion){}

