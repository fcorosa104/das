/********************************
*   Clase Pedido      			*
*   Fichero implmnt.            *
*   Autor: FcoJ Rosa            *
********************************/

#include <iomanip>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "pedido.hpp"
#include "pedido-articulo.hpp"
#include "usuario-pedido.hpp"
using namespace std;

unsigned Pedido::cantPedidos_ = 1;

Pedido::Pedido(Usuario_Pedido& asociacionUP, Pedido_Articulo& asociacionPA,
			   Usuario& usuario, const Tarjeta& tarjeta, const Fecha& fechaPedido):
					numPedido_(cantPedidos_),
					tarjetaPago_(&tarjeta),
					fechaPedido_(fechaPedido)
{
	double total = 0;
	vector<Articulo*> LDExp; //punteros auxiliares a libros expirados para eliminarlos

	if(usuario.tarjetas().find(tarjeta.numero()) == usuario.tarjetas().end())
		throw Impostor(usuario);
	if(tarjeta.titular() != &usuario)
		throw Impostor(usuario);
	if(tarjeta.caducidad() < fechaPedido) //necesita estar aqui, si no las pruebas dan error
		throw Tarjeta::Caducada(tarjeta.caducidad());
	if(!tarjeta.activa())
		throw Tarjeta::Desactivada();
	for(auto it = usuario.compra().begin(); it != usuario.compra().end(); ++it)
	{
		if(ArticuloAlmacenable* pAA = dynamic_cast<ArticuloAlmacenable*>(it->first))
		{
			if(it->second > pAA->stock())
			{
				Articulo* pAux = it->first;
				VaciarCarrito(usuario);
				throw SinStock(*pAux);
			}
		}
		else if(LibroDigital* pLD = dynamic_cast<LibroDigital*>(it->first))
		{
			if(pLD->f_expir() < Fecha())
				LDExp.push_back(it->first);
		}
		/*if(it->second > it->first->stock())
		{
			Articulo* pAux = it->first;
			VaciarCarrito(usuario);
			throw SinStock(*pAux);
		}*/
	}
	for(auto it = LDExp.begin(); it != LDExp.end(); ++it)
		usuario.compra(*(*it), 0);
	if(usuario.compra().size() == 0)
		throw Vacio(usuario);

	for(auto it = usuario.compra().begin(); it != usuario.compra().end(); ++it)
	{
		asociacionPA.pedir(*this, *(it->first), it->first->precio(), it->second);
		asociacionPA.pedir(*(it->first), *this, it->first->precio(), it->second);
		total += ((it->first->precio()) * (it->second));
		if(ArticuloAlmacenable* pAA = dynamic_cast<ArticuloAlmacenable*>(it->first))
			pAA->stock() -= it->second;
	}
	asociacionUP.asocia(usuario, *this);
	asociacionUP.asocia(*this, usuario);
	importeTotalVenta_ = total;
	VaciarCarrito(usuario);
	cantPedidos_++;
}

void Pedido::VaciarCarrito(Usuario& usuario)
{
	auto compra = usuario.compra();
	for(auto it = compra.begin(); it != compra.end(); ++it)
		usuario.compra(*(it->first), 0);
}

ostream& operator<<(std::ostream& os, const Pedido& p)
{
	os << "Núm. pedido: " << p.numero() << endl
	<< "Fecha:       " << p.fecha() << endl
	<< "Pagado con:  ";
	switch(p.tarjeta()->tipo())
    {
        case Tarjeta::VISA: os << "VISA"; break;
        case Tarjeta::Mastercard: os << "Mastercard"; break;
        case Tarjeta::Maestro: os << "Maestro"; break;
        case Tarjeta::JCB: os << "JCB"; break;
        case Tarjeta::AmericanExpress: os << "American Express"; break;
        default: os << "Tipo indeterminado";
    }
    os << " n.º: " << p.tarjeta()->numero() << endl
	<< "Importe:     " << fixed << setprecision(2) << p.total() << " €" << endl;
	return os;
}
