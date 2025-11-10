/******************************************
*   Clase LineaPedido y Pedido_Articulo   *
*   Fichero implmnt.                      *
*   Autor: FcoJ Rosa                      *
******************************************/

#include <iomanip>
#include "pedido-articulo.hpp"
using namespace std;

ostream& operator<<(ostream& os, const LineaPedido& lp)
{
	os << fixed << setprecision(2) << lp.precio_venta() << " €\t" << lp.cantidad() << endl;
	return os;
}

void Pedido_Articulo::pedir(Pedido& p, Articulo& a, double precio, unsigned cantidad)
{
	asociacionPA_[&p].insert(make_pair(&a, LineaPedido(precio,cantidad)));
}

void Pedido_Articulo::pedir(Articulo& a, Pedido& p, double precio, unsigned cantidad)
{
	asociacionAP_[&a].insert(make_pair(&p, LineaPedido(precio,cantidad)));
}

ostream& operator<<(ostream& os, const Pedido_Articulo::ItemsPedido& ip)
{
	double total = 0;
	os << "  PVP   Cantidad        Artículo" << endl <<
	"==================================================================" << endl;
    for(auto it = ip.begin(); it != ip.end(); ++it)
    {
        os << fixed << setprecision(2) << it->second.precio_venta() << " € "
        << it->second.cantidad() << "\t\t[" << it->first->referencia() << "] "
        << "\"" << it->first->titulo() << "\"" << endl;
        total += ((it->second.precio_venta()) * (it->second.cantidad()));
    }
	os << "==================================================================" << endl <<
	"Total   " << fixed << setprecision(2) << total << " €" << endl;
	return os;
}

ostream& operator<<(ostream& os, const Pedido_Articulo::Pedidos& ps)
{
	double total = 0;
	unsigned cTotal = 0;
	os << "[Pedidos: " << ps.size() << "]" << endl <<
	"==================================================================" << endl <<
	"  PVP   Cantidad           Fecha de venta" << endl <<
	"==================================================================" << endl;
    for(auto it = ps.begin(); it != ps.end(); ++it)
    {
        os << fixed << setprecision(2) << it->second.precio_venta() << " € "
        << it->second.cantidad() << "\t\t" << it->first->fecha() << endl;
        total += ((it->second.precio_venta()) * (it->second.cantidad()));
        cTotal += it->second.cantidad();
    }
    os << fixed << setprecision(2) << total << " €\t" << cTotal << endl;
	return os;
}

ostream& Pedido_Articulo::mostrarDetallePedidos(ostream& os)
{
	double total = 0;
    for(auto it = asociacionPA_.begin(); it != asociacionPA_.end(); ++it)
    {
    	os << "Pedido núm. " << it->first->numero() << endl <<
    	"Cliente: " << it->first->tarjeta()->titular()->nombre() << "\t\t" <<
    	it->first->fecha() << endl << it->second;
    	total += it->first->total();
    }
    os << "TOTAL VENTAS\t\t" << fixed << setprecision(2) << total << " €" << endl;
	return os;
}

ostream& Pedido_Articulo::mostrarVentasArticulos(ostream& os)
{
	for(auto it = asociacionAP_.begin(); it != asociacionAP_.end(); ++it)
	{
		os << "Ventas de [" << it->first->referencia() << "] "
        << "\"" << it->first->titulo() << "\"" << endl;
        os << it->second;
	}
	return os;
}
