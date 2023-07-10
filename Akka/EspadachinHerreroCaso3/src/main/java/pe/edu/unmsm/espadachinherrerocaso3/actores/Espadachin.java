/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm.espadachinherrerocaso3.actores;

import akka.actor.UntypedAbstractActor;
import akka.event.Logging;
import akka.event.LoggingAdapter;
import pe.edu.unmsm.espadachinherrerocaso3.Main;

/**
 *
 * @author nicol
 */
public class Espadachin extends UntypedAbstractActor{
    public enum Mensaje {
        ESPADA_NUEVA,
        ESPADA_ROTA
    }
 
    private final LoggingAdapter log = Logging.getLogger(getContext().system(), this);
 
    @Override
    public void onReceive(Object o) {
        log.info("[Espadachin] ha recibido el mensaje: \"{}\".", o);
 
        if (o == Mensaje.ESPADA_ROTA) {
            Main.herrero.tell(Herrero.Mensaje.CREAR_ESPADA, getSelf());
        } else if (o == Mensaje.ESPADA_NUEVA) {
            getContext().stop(getSelf());
        } else {
            unhandled(o);
        }
    }
}
