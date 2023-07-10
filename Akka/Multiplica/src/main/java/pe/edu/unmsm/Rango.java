/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pe.edu.unmsm;

import akka.actor.UntypedAbstractActor;

/**
 *
 * @author vhbo
 */
public class Rango extends UntypedAbstractActor {
    
    public static final class Mensaje {
        private int num;
        private int[] vec;
        private int ini;
        private int fin;
        public Mensaje(int num, int[] vec, int ini, int fin) {
            this.num = num;
            this.vec = vec;
            this.ini = ini;
            this.fin = fin;
        }
        public int getNum() {
            return num;
        }
        public int[] getVec() {
            return vec;
        }
        public int getIni() {
            return ini;
        }
        public int getFin() {
            return fin;
        }
    }

    @Override
    public void onReceive(Object message) throws Throwable {
        if ( message instanceof Mensaje ) {
            Mensaje mes = (Mensaje)message;
            int sum = 0;
            for( int i=mes.getIni(); i<mes.getFin(); i++) {
                sum += mes.getVec()[i] * mes.getNum();
            }
            getSender().tell(sum, getSelf());
        }
    }
    
    
}
