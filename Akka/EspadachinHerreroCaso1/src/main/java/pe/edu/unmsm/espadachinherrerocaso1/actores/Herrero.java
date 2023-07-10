/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm.espadachinherrerocaso1.actores;

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
        CREAR_ESPADA
    }
 
    private final LoggingAdapter log = Logging.getLogger(getContext().system(), this);
    private ActorRef minero;
 
 
    @Override
    public void preStart() {
        minero = getContext().actorOf(Props.create(Minero.class), "minero");
    }
 
    @Override
    public void onReceive(Object message) throws InterruptedException {
        log.info("[Herrero] ha recibido el mensaje: \"{}\".", message);
 
        if (message == Mensaje.CREAR_ESPADA) {
            minero.tell(Minero.Mensaje.OBTENER_MATERIALES, getSelf());
        }
    }
}
