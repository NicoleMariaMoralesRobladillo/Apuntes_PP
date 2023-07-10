/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Main.java to edit this template
 */
package pe.edu.unmsm;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;

/**
 *
 * @author vhbo
 */
public class Inicio {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        int vector[] = {1,2,3,4,5,6,7,8,9,10};
        int numeroMultiplicar = 2;
        ActorSystem actorSystem = ActorSystem.create();
        try {
            ActorRef actor = actorSystem.actorOf(Props.create(Multiplica.class), "Yo");
            actor.tell(new Multiplica.Mensaje(numeroMultiplicar, vector), ActorRef.noSender());
        }
        finally {
            actorSystem.terminate();
        }
        
    }
    
}
