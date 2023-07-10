/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm.espadachinherreromod.actores;

import akka.actor.ActorRef;
import akka.actor.UntypedAbstractActor;
import akka.event.Logging;
import akka.event.LoggingAdapter;
import pe.edu.unmsm.espadachinherreromod.Main;

/**
 *
 * @author nicol
 */
public class Espadachin extends UntypedAbstractActor {
    public enum Mensaje {
        ESPADA_NUEVA,
        ESPADA_ROTA
    }
 
    private final LoggingAdapter log = Logging.getLogger(getContext().system(), this);
    private ActorRef dios;

    @Override
    public void onReceive(Object message) {
        log.info("[Espadachin] ha recibido el mensaje: \"{}\".", message);
        if (message == Mensaje.ESPADA_ROTA) {
            this.dios = getSender();
            Main.herrero.tell(Herrero.Mensaje.CREAR_ESPADA, getSelf());
        }
        else if (message == Mensaje.ESPADA_NUEVA) {
            getContext().stop(getSelf());
            this.dios.tell(Dios.Mensaje.ESPADA_LISTA, ActorRef.noSender());
        } else {
            unhandled(message);
        }
    }
}
