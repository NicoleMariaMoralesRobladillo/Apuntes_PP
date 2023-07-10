/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm.espadachinherreromod.actores;

import akka.actor.ActorRef;
import akka.actor.UntypedAbstractActor;
import akka.event.Logging;
import akka.event.LoggingAdapter;
import java.util.ArrayList;
import pe.edu.unmsm.espadachinherreromod.Main;

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
    private ArrayList<ActorRef> espadachines;

    @Override
    public void onReceive(Object message) throws InterruptedException {
        log.info("[Herrero] ha recibido el mensaje: \"{}\".", message);
 
        if (message == Mensaje.CREAR_ESPADA) {
            this.espadachines.add(getSender());
            Main.minero.tell(Minero.Mensaje.OBTENER_MATERIALES, getSelf());
        }
        else if (message == Mensaje.MATERIALES) {
            log.info("[Herrero] esta creando espada...");
            crearEspada();
            log.info("[Herrero] ha creado espada.");
            if (!this.espadachines.isEmpty()) {
                this.espadachines.get(0).tell(Espadachin.Mensaje.ESPADA_NUEVA, ActorRef.noSender());
                this.espadachines.remove(0);
            }
        }
        else {
            unhandled(message);
        }
    }

    @Override
    public void preStart() {
        this.espadachines = new ArrayList<>();
    }
    
    private void crearEspada() throws InterruptedException {
        Thread.sleep(TIEMPO_CREACION_ESPADA);
    }
}
