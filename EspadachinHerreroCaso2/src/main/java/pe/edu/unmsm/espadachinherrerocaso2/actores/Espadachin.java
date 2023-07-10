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
public class Espadachin extends UntypedAbstractActor{
    public enum Mensaje {
        ESPADA_NUEVA,
        SALUDO
    }
 
    private final LoggingAdapter log = Logging.getLogger(getContext().system(), this);
 
    @Override
    public void preStart() {
        final ActorRef herrero = getContext().actorOf(Props.create(Herrero.class), "herrero");
        herrero.tell(Herrero.Mensaje.CREAR_ESPADA, getSelf());
        herrero.tell(Mensaje.SALUDO, getSelf());
    }
 
    @Override
    public void onReceive(Object message) {
        log.info("[Espadachin] ha recibido el mensaje: \"{}\".", message);
 
        if (message == Mensaje.ESPADA_NUEVA) {
            getContext().stop(getSelf());
        } else {
            unhandled(message);
        }
    }
}
