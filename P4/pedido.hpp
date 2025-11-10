/********************************
*   Clase Pedido      			*
*   Fichero cabecera            *
*   Autor: FcoJ Rosa            *
********************************/

#ifndef PEDIDO_HPP
#define PEDIDO_HPP
#include "tarjeta.hpp"

class Usuario_Pedido;
class Pedido_Articulo;

class Pedido
{
public:
	Pedido(Usuario_Pedido&, Pedido_Articulo&, Usuario&, const Tarjeta&, const Fecha& fechaPedido=Fecha());
	unsigned numero() const{return numPedido_;}
	const Tarjeta* tarjeta() const{return tarjetaPago_;}
	const Fecha& fecha() const{return fechaPedido_;}
	double total() const{return importeTotalVenta_;}
	static unsigned n_total_pedidos() {return cantPedidos_;}
    class Vacio
    {
    public:
        Vacio(const Usuario& u):usuario_(&u){}
        const Usuario& usuario() const{return *usuario_;}
    private:
        const Usuario* usuario_;
    };
    class Impostor
    {
    public:
        Impostor(const Usuario& u):usuario_(&u){}
        const Usuario& usuario() const{return *usuario_;}
    private:
        const Usuario* usuario_;
    };
    class SinStock
    {
    public:
        SinStock(const Articulo& a):articulo_(&a){}
        const Articulo& articulo() const{return *articulo_;}
    private:
        const Articulo* articulo_;
    };
private:
    void VaciarCarrito(Usuario& usuario);
	unsigned numPedido_;
	const Tarjeta* tarjetaPago_;
	Fecha fechaPedido_;
	double importeTotalVenta_;
	static unsigned cantPedidos_;
};

std::ostream& operator<<(std::ostream& os, const Pedido& p);

#endif //PEDIDO_HPP
