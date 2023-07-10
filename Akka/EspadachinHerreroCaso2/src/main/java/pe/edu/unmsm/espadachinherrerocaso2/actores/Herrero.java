/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm.espadachinherrerocaso2.actores;

import akka.actor.ActorRef;
import akka.actor.Props;
import akka.actor.UntypedAbstractActor;
import akka.event.Logging;
import akka.event.LoggingAdapter;

/**
 *
 * @author nicol
 */
public class Herrero extends UntypedAbstractActor{
    public enum Mensaje {
        CREAR_ESPADA,
        MATERIALES
    }
 
    private final LoggingAdapter log = Logging.getLogger(getContext().system(), this);
 
    private static final long TIEMPO_CREACION_ESPADA = 2000;
    private ActorRef espadachin;
    private ActorRef minero;
 
 
    @Override
    public void preStart() {
        minero = getContext().actorOf(Props.create(Minero.class), "minero");
    }
 
    @Override
    public void onReceive(Object message) throws InterruptedException {
        log.info("[Herrero] ha recibido el mensaje: \"{}\".", message);
 
        if (message == Mensaje.CREAR_ESPADA) {
            espadachin = getSender();
            minero.tell(Minero.Mensaje.OBTENER_MATERIALES, getSelf());
        } else if (message == Mensaje.MATERIALES) {
            log.info("[Herrero] esta creando espada...");
            crearEspada();
            log.info("[Herrero] ha creado espada.");
            espadachin.tell(Espadachin.Mensaje.ESPADA_NUEVA, getSelf());
        } else {
            unhandled(message);
        }
    }
 
    private void crearEspada() throws InterruptedException {
        Thread.sleep(TIEMPO_CREACION_ESPADA);
    }
 
    @Override
    public void unhandled(Object message) {
        log.info("[Herrero] no sabe qué hacer ante el mensaje: \"{}\".", message);
    }
}
