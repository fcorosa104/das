/******************************************
*   Clase LineaPedido y Pedido_Articulo   *
*   Fichero cabecera                      *
*   Autor: FcoJ Rosa                      *
******************************************/

#ifndef PEDIDOARTICULO_HPP
#define PEDIDOARTICULO_HPP
#include <iostream>
#include <iomanip>
#include "pedido.hpp"
#include "articulo.hpp"

class LineaPedido
{
public:
	explicit LineaPedido(double pDV, unsigned cV = 1):precioDeVenta_(pDV),cantidadVendida_(cV){}
	double precio_venta() const{return precioDeVenta_;}
	unsigned cantidad() const{return cantidadVendida_;}
private:
	double precioDeVenta_;
	unsigned cantidadVendida_;
};

std::ostream& operator<<(std::ostream& os, const LineaPedido& lp);

struct OrdenaPedidos
{
	bool operator()(Pedido* p1, Pedido* p2) const{return p1->numero() < p2->numero();}
};

struct OrdenaArticulos
{
	bool operator()(Articulo* a1, Articulo* a2) const{return a1->referencia() < a2->referencia();}
};

class Pedido_Articulo
{
public:
	typedef std::map<Articulo*, LineaPedido, OrdenaArticulos> ItemsPedido;
	typedef std::map<Pedido*, LineaPedido, OrdenaPedidos> Pedidos;
	void pedir(Pedido& p, Articulo& a, double precio, unsigned cantidad=1);
	void pedir(Articulo& a, Pedido& p, double precio, unsigned cantidad=1);
	ItemsPedido& detalle(Pedido& p){return asociacionPA_[&p];}
	Pedidos& ventas(Articulo& a){return asociacionAP_[&a];}
	std::ostream& mostrarDetallePedidos(std::ostream& os);
	std::ostream& mostrarVentasArticulos(std::ostream& os);
private:
	std::map<Pedido*, ItemsPedido, OrdenaPedidos> asociacionPA_;
	std::map<Articulo*, Pedidos, OrdenaArticulos> asociacionAP_;
};

std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::ItemsPedido& ip);
std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::Pedidos& ps);

#endif //PEDIDOARTICULO_HPP
