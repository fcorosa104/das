/********************************
*   Clase UsuarioPedido			*
*   Fichero cabecera            *
*   Autor: FcoJ Rosa            *
********************************/
	//UsuarioPedido solo tiene cabecera

#ifndef USUARIOPEDIDO_HPP
#define USUARIOPEDIDO_HPP
#include <map>
#include <set>
#include "usuario.hpp"
#include "pedido.hpp"
#include "pedido-articulo.hpp"

class Usuario_Pedido
{
public:
	typedef std::set<Pedido*> Pedidos;
	void asocia(Usuario& u, Pedido& p){asociacionUP_[&u].insert(&p);}
	void asocia(Pedido& p, Usuario& u){asociacionPU_.insert(std::make_pair(&p, &u));}
	Pedidos pedidos(Usuario& u){return asociacionUP_[&u];}
	Usuario* cliente(Pedido& p){return asociacionPU_[&p];}
private:
	std::map<Usuario*, Pedidos> asociacionUP_;
	std::map<Pedido*, Usuario*> asociacionPU_;
};

#endif //USUARIOPEDIDO_HPP
