#include "mycliente.h"

Mycliente::Mycliente(QObject*parent):QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(15);
}
void Mycliente::SetSocket(qintptr Desc)
{
    socket=new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));


    socket->setSocketDescriptor(Desc);
    qDebug()<<"Cliente conectado";
}
void Mycliente::connected()
{
    qDebug()<<"Cliente conectado ev";
}
void Mycliente::disconnected()
{
    qDebug()<<"Cliente no conectado";
}
void Mycliente::readyRead()
{
    QByteArray data=socket->readAll();
    if(data.toStdString().substr(0,2)=="LO")
    {
        conectadoE(data);
    }
    else if(data.toStdString().substr(0,2)=="RE")
    {
        registrarCliente(data);
    }
    else if(data.toStdString().substr(0,2)=="AD")
    {
        string ced=data.toStdString().substr(2,data.toStdString().length()-2);
        clienodo aux=clienteslog.primero;
        while(aux!=nullptr)
        {
            if(aux->cedula==ced)
            {
                colaclientes.insertarFinal(aux->cedula,aux->nombre,aux->telefono,aux->correo,aux->carrito,0,aux->socket);
                cout<<"agregado"<<endl;

            }
                aux=aux->siguiente;
        }
    }
    else if(data.toStdString().substr(0,2)=="CO")
    {
        if(data.toStdString().substr(2,2)=="PA")
        {
        string send="COPA;";
        string arbol;
        arbol=inordenMandar(supermercado.raiz);
        send.append(arbol);
        this->write(QByteArray::fromStdString(send));
        }
        else if(data.toStdString().substr(2,2)=="PR")
        {
            string datos=data.toStdString();
            char separador[]=";";
            char cstr[datos.size()+1];
            strcpy(cstr,datos.c_str());
            char*token= strtok(cstr,separador);
            token=strtok(nullptr,separador);
            string pasillo=token;
            int pasilloi=std::stoi(pasillo);
            pNodoBinario raiz=supermercado.raiz;
            pNodoBinario pasillop=buscarNodo(raiz,pasilloi);
            if(pasillop!=nullptr)
            {
                string send="COPR;";
                string arbol;
                arbol=inordenMandarP(pasillop->productos);
                send.append(arbol);
                this->write(QByteArray::fromStdString(send));
            }
            else
            {
                QByteArray validacion="VAPA";
                this->write(validacion);
            }

        }
        else if(data.toStdString().substr(2,2)=="MA")
        {
            string datos=data.toStdString();
            char separador[]=";";
            char cstr[datos.size()+1];
            strcpy(cstr,datos.c_str());
            char*token= strtok(cstr,separador);
            token=strtok(nullptr,separador);
            string producto=token;
            int pasillo=stoi(token);
            token=strtok(nullptr,separador);
            int productoi=stoi(token);
            pNodoBinario raiz=supermercado.raiz;
            pNodoBinario pasillon=buscarNodo(raiz,pasillo);
            pNodoBinarioAVL producton=buscarNodoAVL(pasillon->productos,productoi);
            if (producton!=nullptr)
            {
            string send="COMA;";
            string arbol=inordenMandarM(producton->marcas);
            send.append(arbol);
            this->write(QByteArray::fromStdString(send));
            }
            else
            {
                QByteArray validacion="VAPR";
                this->write(validacion);
            }
        }
        else if(data.toStdString().substr(2,2)=="CC")
        {
            string datos=data.toStdString();
            char separador[]=";";
            char cstr[datos.size()+1];
            strcpy(cstr,datos.c_str());
            char*token= strtok(cstr,separador);
            token=strtok(nullptr,separador);
            int pasillo=stoi(token);
            token=strtok(nullptr,separador);
            int productoi=stoi(token);
            token=strtok(nullptr,separador);
            int marcaI=stoi(token);
            pNodoBinario raiz=supermercado.raiz;
            pNodoBinario pasillon=buscarNodo(raiz,pasillo);
            pNodoBinarioAVL producton=buscarNodoAVL(pasillon->productos,productoi);
            RBTree rojo=RBTree(producton->marcas);
            NodePtr marca=rojo.searchTree(marcaI);
            if(marca!=nullptr)
            {
                string send="COCC";
                string buscar=std::to_string(pasillon->valor)+std::to_string(producton->valor)+std::to_string(marca->data);
                nodoAA*inv=inventario.buscarNodoAA(inventario.raiz,stoi(buscar));
                send.append(std::to_string(inv->canastaB));
                this->write(QByteArray::fromStdString(send));
            }
            else
            {
                QByteArray validacion="VAMA";
                this->write(validacion);
            }
        }
        else if(data.toStdString().substr(2,2)=="CI")
        {
            string datos=data.toStdString();
            char separador[]=";";
            char cstr[datos.size()+1];
            strcpy(cstr,datos.c_str());
            char*token= strtok(cstr,separador);
            token=strtok(nullptr,separador);
            int pasillo=stoi(token);
            token=strtok(nullptr,separador);
            int productoi=stoi(token);
            token=strtok(nullptr,separador);
            int marcaI=stoi(token);
            pNodoBinario raiz=supermercado.raiz;
            pNodoBinario pasillon=buscarNodo(raiz,pasillo);
            pNodoBinarioAVL producton=buscarNodoAVL(pasillon->productos,productoi);
            RBTree rojo=RBTree(producton->marcas);
            NodePtr marca=rojo.searchTree(marcaI);
            if(marca!=nullptr)
            {
                string send="COCI";
                string buscar=std::to_string(pasillon->valor)+std::to_string(producton->valor)+std::to_string(marca->data);
                nodoAA*inv=inventario.buscarNodoAA(inventario.raiz,stoi(buscar));
                if(inv->canastaB==0)
                {
                    send.append("El impuesto es de 1%");
                }
                else
                {
                    send.append("El impuesto es de: ");
                    send.append(std::to_string(inv->impuesto));
                    send.append("%");
                }
                this->write(QByteArray::fromStdString(send));
            }
            else
            {
                QByteArray validacion="VAMA";
                this->write(validacion);
            }
        }
        else if(data.toStdString().substr(2,2)=="CN")
        {
            string datos=data.toStdString();
            char separador[]=";";
            char cstr[datos.size()+1];
            strcpy(cstr,datos.c_str());
            char*token= strtok(cstr,separador);
            token=strtok(nullptr,separador);
            int pasillo=stoi(token);
            token=strtok(nullptr,separador);
            int productoi=stoi(token);
            token=strtok(nullptr,separador);
            int marcaI=stoi(token);
            pNodoBinario raiz=supermercado.raiz;
            pNodoBinario pasillon=buscarNodo(raiz,pasillo);
            pNodoBinarioAVL producton=buscarNodoAVL(pasillon->productos,productoi);
            RBTree rojo=RBTree(producton->marcas);
            NodePtr marca=rojo.searchTree(marcaI);
            if(marca!=nullptr)
            {
                string send="COCA";
                send.append(std::to_string(marca->precio));
                this->write(QByteArray::fromStdString(send));
            }
            else
            {
                QByteArray validacion="VAMA";
                this->write(validacion);
            }
        }
        else if(data.toStdString().substr(2,2)=="CO")
        {
            string datos=data.toStdString();
            char separador[]=";";
            char cstr[datos.size()+1];
            strcpy(cstr,datos.c_str());
            char*token= strtok(cstr,separador);
            token=strtok(nullptr,separador);
            int pasillo=stoi(token);
            token=strtok(nullptr,separador);
            int productoi=stoi(token);
            token=strtok(nullptr,separador);
            int marcaI=stoi(token);
            pNodoBinario raiz=supermercado.raiz;
            pNodoBinario pasillon=buscarNodo(raiz,pasillo);
            pNodoBinarioAVL producton=buscarNodoAVL(pasillon->productos,productoi);
            RBTree rojo=RBTree(producton->marcas);
            NodePtr marca=rojo.searchTree(marcaI);
            if(marca!=nullptr)
            {
                string send="COCO;";
                string arbol=std::to_string(marca->cantidadGondola);
                send.append(arbol);
                this->write(QByteArray::fromStdString(send));
            }
            else
            {
                QByteArray validacion="VAMA";
                this->write(validacion);
            }
        }
        else if(data.toStdString().substr(2,2)=="AG")
        {
            string datos=data.toStdString();
            char separador[]=";";
            char cstr[datos.size()+1];
            strcpy(cstr,datos.c_str());
            char*token= strtok(cstr,separador);
            token=strtok(nullptr,separador);
            int pasillo=stoi(token);
            token=strtok(nullptr,separador);
            int productoi=stoi(token);
            token=strtok(nullptr,separador);
            int marcaI=stoi(token);
            pNodoBinario raiz=supermercado.raiz;
            pNodoBinario pasillon=buscarNodo(raiz,pasillo);
            pNodoBinarioAVL producton=buscarNodoAVL(pasillon->productos,productoi);
            RBTree rojo=RBTree(producton->marcas);
            NodePtr marca=rojo.searchTree(marcaI);
            token=strtok(nullptr,separador);
            int cantidad=stoi(token);
            token=strtok(nullptr,separador);
            int cedula=stoi(token);
            if(cantidad>marca->cantidadGondola)
            {
                cantidad=marca->cantidadGondola;
                marca->cantidadGondola=0;
                marca->cantidadVentas=marca->cantidadVentas+cantidad;
            }
            else
            {
                marca->cantidadGondola=marca->cantidadGondola-cantidad;
                marca->cantidadVentas=marca->cantidadVentas+cantidad;
            }
            clienodo aux=clienteslog.primero;
            while(aux!=nullptr)
            {
                if(aux->cedula==std::to_string(cedula))
                {
                    std::cout<<aux->nombre<<std::endl;
                    nodoCompra*prod=aux->carrito->primero;
                    while(prod!=nullptr)
                    {
                        if(prod->nombre==marca->nombre)
                        {
                            prod->cantidad=prod->cantidad+cantidad;
                            return;
                        }
                        prod=prod->siguiente;
                    }
                    aux->carrito->InsertarInicio(std::to_string(pasillo),std::to_string(productoi),std::to_string(marcaI),marca->nombre,cantidad);
                    aux=aux->siguiente;
                }
            }
        }
    }
}

void Mycliente::write(QByteArray data)
{
        // Must always be called on thread 1
        this->socket->write(data);
}
void Mycliente::conectadoE(QByteArray data)
{
std::string cedula=data.toStdString().substr(2,7);
Pagina*cliente=new Pagina(5);
int k;
cliente=clientes.buscar(std::stoi(cedula),k);
if (cliente!=nullptr)
{
    cout<<this->socket->socketDescriptor()<<endl;
    socket->write("LOS");
    PilaC*carrito=new PilaC;
    clienteslog.insertarFinal(cedula,cliente->obtenerDato(k,0),cliente->obtenerDato(k,1),cliente->obtenerDato(k,2),carrito,0,this->socket->socketDescriptor());
}
else
{
    socket->write("LON");
}
}
void Mycliente::registrarCliente(QByteArray data)
{
    string datos=data.toStdString();
    char separador[]=";";
    char cstr[datos.size()+1];
    strcpy(cstr,datos.c_str());
    char*token= strtok(cstr,separador);
    string cedula=token;
    token=strtok(nullptr,separador);
    string nombre=token;
    token=strtok(nullptr,separador);
    string ciudad=token;
    token=strtok(nullptr,separador);
    string numero=token;
    token=strtok(nullptr,separador);
    string correo=token;
    cout<<cedula<<endl;
    cedula=cedula.substr(2,7);
    int cedulaS = std::stoi(cedula);
    int n;
    Pagina* aux = clientes.buscar(cedulaS,n);
    if (aux == NULL){
        clientes.insertar(cedulaS,nombre,numero,ciudad,correo);
        aux->cambiarEstadistica(n,0,aux->obtenerEstadistica(n,0)+1);
        socket->write("RGY");

    }
    else
    {
        socket->write("RGN");
    }
}
void Mycliente::setCedula(int cedula)
{
    this->cedula=cedula;
}
string Mycliente::inordenMandar(pNodoBinario nodo)
{
    if(nodo==NULL){
            return "";
    }else
    {   string aux;
        aux.append("Nombre: ");
        aux.append(nodo->nombre);
        aux.append(" Codigo: ");
        aux.append(std::to_string(nodo->valor));
        aux.append("\n");
        aux+=inordenMandar(nodo->Hizq);
        aux+=inordenMandar(nodo->Hder);
        return aux;
    }
}
string Mycliente::inordenMandarP(pNodoBinarioAVL nodo)
{
    if(nodo==NULL){
            return "";
    }else
    {   string aux;
        aux.append("Nombre: ");
        aux.append(nodo->nombre);
        aux.append(" Codigo: ");
        aux.append(std::to_string(nodo->valor));
        aux.append("\n");
        aux+=inordenMandarP(nodo->Hizq);
        aux+=inordenMandarP(nodo->Hder);
        return aux;
    }
}
string Mycliente::inordenMandarM(NodePtr nodo)
{
    if(nodo==NULL){
            return "";
    }else
    {   string aux;
        aux.append("Nombre: ");
        aux.append(nodo->nombre);
        aux.append(" Codigo: ");
        aux.append(std::to_string(nodo->data));
        aux.append("Precio: ");
        aux.append(std::to_string(nodo->precio));
        aux.append("\n");
        aux+=inordenMandarM(nodo->left);
        aux+=inordenMandarM(nodo->right);
        return aux;
    }
}

