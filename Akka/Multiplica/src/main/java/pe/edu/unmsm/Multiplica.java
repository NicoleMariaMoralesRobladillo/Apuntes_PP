/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm;

import akka.actor.ActorRef;
import akka.actor.Props;
import akka.actor.UntypedAbstractActor;
import java.util.ArrayList;

/**
 *
 * @author vhbo
 */
public class Multiplica extends UntypedAbstractActor {
    
    private int total = 0;
    private int contadorRangosListos = 0;
    private final int cantidadRangos = 5;
    ArrayList<ActorRef> rangos;
    
    public static final class Mensaje {
        private final int num;
        private final int[] vec;
        public Mensaje(int num, int[] vec) {
            this.num = num;
            this.vec = vec;
        }
        public int getNum() {
            return num;
        }
        public int[] getVec() {
            return vec;
        }
    }
    
    @Override
    public void onReceive(Object message) throws Throwable {
        if ( message instanceof Mensaje ) {
            Mensaje mes = (Mensaje)message;
            int bloque = mes.getVec().length/cantidadRangos;
            for(int i=0; i<cantidadRangos; i++){
                rangos.add(getContext().actorOf(Props.create(Rango.class), "Rango-"+i));
                rangos.get(i).tell(new Rango.Mensaje(mes.getNum(),mes.getVec(),i*bloque,(i+1)*bloque), getSelf());
            }
        }
        
        if ( message instanceof Integer ) {
            int sum = (Integer) message;
            total += sum;
            contadorRangosListos++;
            System.err.println(
               "Recibido " + contadorRangosListos + " de " + getSender().path().name()
            );
            if ( contadorRangosListos == cantidadRangos ) {
                System.err.println("El resultado es " + total);
                getContext().stop(getSelf());
            }
        }
        
    }

    @Override
    public void preStart() throws Exception {
        rangos = new ArrayList<>();
    }
}
