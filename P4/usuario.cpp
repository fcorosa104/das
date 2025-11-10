/********************************
*   Clase Clave y Usuario       *
*   Fichero implmnt.            *
*   Autor: FcoJ Rosa            *
********************************/

#include <cstring>
#include <unistd.h>
#include <random>
#include <ctime>
#include <unordered_set>
#include <iomanip>
#include "usuario.hpp"
using namespace std;

unordered_set<Cadena> Usuario::identificadores_;

Clave::Clave(const char* contrasenna)
{
    if(strlen(contrasenna) < 5) throw Incorrecta(CORTA);
    random_device rDevice;
    char alfabeto[] = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"};
    uniform_int_distribution<int> intDist(0,strlen(alfabeto)-1);
    Cadena semilla;
    for(int i=0; i<10; i++)
        semilla += Cadena(1,alfabeto[intDist(rDevice)]);
    char* cifrado = crypt(contrasenna, semilla.c_str());
    if(cifrado == nullptr) throw Incorrecta(ERROR_CRYPT);
    contrasenna_ = cifrado;
}

bool Clave::verifica(const char* contrasenna) const
{
    return strcmp(contrasenna_.c_str(), crypt(contrasenna, contrasenna_.c_str())) == 0;
}

Usuario::Usuario(const Cadena& identificador, const Cadena& nombre, const Cadena& apellidos,
                 const Cadena& direccion, const Clave& clave):
    identificador_(identificador),
    nombre_(nombre),
    apellidos_(apellidos),
    direccionUsuario_(direccion),
    contrasenna_(clave)
{
    if(!identificadores_.insert(identificador).second)
        throw Id_duplicado(identificador);
}

void Usuario::es_titular_de(const Tarjeta& t)
{
    //Asociar usuario con tarjeta
    if(t.titular() == this && !t.activa())
        tarjetas_.insert(make_pair(t.numero(),const_cast<Tarjeta*>(&t)));
}

void Usuario::no_es_titular_de(const Tarjeta& t)
{
    //Desasociar usuario con tarjeta
    tarjetas_.erase(t.numero());
}

Usuario::~Usuario()
{
    //Llamar metodo anula_titular sobre cada tarjeta
    for(auto it = tarjetas_.begin(); it != tarjetas_.end(); ++it)
        it->second->anula_titular();
    //MIRAR SOLO USUARIO PODRA LLAMAR A ESTE METODO DE TARJETA
    //
    identificadores_.erase(identificador_);
}

void Usuario::compra(const Articulo& a, unsigned cantidad)
{
    //Asociacion unidireccional con articulo
    Articulos::iterator it = articulos_.find(const_cast<Articulo*>(&a));
    if(it == articulos_.end())
    {
        if(cantidad != 0)
            articulos_.insert(make_pair(const_cast<Articulo*>(&a),cantidad));
    }
    else
    {
        if(cantidad == 0)
            articulos_.erase(articulos_.find(const_cast<Articulo*>(&a)));
        else
            it->second = cantidad;
    }
}

unsigned Usuario::n_articulos() const
{
    //Devolver el n de articulos diferentes
    unsigned nArts = 0;
    for(auto it = articulos_.begin(); it != articulos_.end(); ++it)
        nArts++;
    return nArts;
}

ostream& operator<<(ostream& os, const Usuario& u)
{
    os << u.id() << " [" << u.contrasenna_.clave() << "] " << u.nombre() << " " << u.apellidos() << endl;
    os << u.direccion() << endl;
    os << "Tarjetas: " << endl;
    for(auto it = u.tarjetas().begin(); it != u.tarjetas().end(); ++it)
        os << *(it->second);
    return os;
}

ostream& mostrar_carro(ostream& os, const Usuario& u)
{
    os << "Carrito de la compra de " << u.id() << " [Artículos: " << u.n_articulos() << "]" << endl;
    os << " Cant. Artículo" << endl;
    os << "===========================================================" << endl;
    for(auto it = u.compra().begin(); it != u.compra().end(); ++it)
    {
        os << "   " << it->second << "   " << "[" << it->first->referencia() << "] " << "\"" << it->first->titulo() <<
        "\", " << it->first->f_publi().anno() << ". " << fixed << setprecision(2) << it->first->precio() << " €" << endl;
    }
    return os;
}
