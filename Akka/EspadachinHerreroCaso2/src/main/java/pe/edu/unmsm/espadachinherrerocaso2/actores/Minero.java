/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm.espadachinherrerocaso2.actores;

import akka.actor.UntypedAbstractActor;
import akka.event.Logging;
import akka.event.LoggingAdapter;

/**
 *
 * @author nicol
 */
public class Minero extends UntypedAbstractActor{
    public enum Mensaje {
        OBTENER_MATERIALES
    }
 
    private final LoggingAdapter log = Logging.getLogger(getContext().system(), this);
    private static final long TIEMPO_OBTENCION_MATERIALES = 2000;
 
    @Override
    public void onReceive(Object message) throws InterruptedException {
        log.info("[Minero] ha recibido el mensaje: \"{}\".", message);
 
        if (message == Mensaje.OBTENER_MATERIALES) {
            log.info("[Minero] esta obteniendo materiales...");
            obtenerMinerales();
            log.info("[Minero] ha obtenido materiales.");
            getSender().tell(Herrero.Mensaje.MATERIALES, getSelf());
        } else {
            unhandled(message);
        }
    }
 
    private void obtenerMinerales() throws InterruptedException {
        Thread.sleep(TIEMPO_OBTENCION_MATERIALES);
    }
}
