/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm.espadachinherreromod.actores;

import akka.actor.UntypedAbstractActor;
import akka.event.Logging;
import akka.event.LoggingAdapter;
import pe.edu.unmsm.espadachinherreromod.Main;

/**
 *
 * @author nicol
 */
public class Dios extends UntypedAbstractActor{
    public enum Mensaje {
        INICIA,
        ESPADA_LISTA
    }
    private final LoggingAdapter log = Logging.getLogger(getContext().system(), this);
    private static final long TIEMPO_ESPERA = 1000;
    private int cuentaEspadachinesEnviados = 0;
    @Override
    public void preStart() throws Exception {
        super.preStart();
    }
    
    @Override
    public void onReceive(Object message) throws Throwable {
        log.info("[Dios] ha recibido el mensaje: \"{}\".", message);
        if (message == Mensaje.INICIA) {
            for(int i=0; i<Main.CANTIDAD_ESPADACHINES; i++){
                cuentaEspadachinesEnviados++;
                Main.espadachines.get(i).tell(Espadachin.Mensaje.ESPADA_ROTA, getSelf());
                if(i!= Main.espadachines.size() - 1) {
                    Thread.sleep(TIEMPO_ESPERA);
                }
            }
        }
        else if (message == Mensaje.ESPADA_LISTA) {
            cuentaEspadachinesEnviados--;
            if(cuentaEspadachinesEnviados <1){
                getContext().getSystem().stop(getSelf());
                Main.actorSystem.terminate();
            }
        }
        else {
            unhandled(message);
        }
    }
}
