/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Project/Maven2/JavaApp/src/main/java/${packagePath}/${mainClassName}.java to edit this template
 */

package pe.edu.unmsm.espadachinherreromod;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import java.util.ArrayList;
import pe.edu.unmsm.espadachinherreromod.actores.Dios;
import pe.edu.unmsm.espadachinherreromod.actores.Espadachin;
import pe.edu.unmsm.espadachinherreromod.actores.Herrero;
import pe.edu.unmsm.espadachinherreromod.actores.Minero;

/**
 *
 * @author nicol
 */
public class Main {
    public static final int CANTIDAD_ESPADACHINES = 4;
    public static ActorRef dios;
    public static ActorRef herrero;
    public static ActorRef minero;
    public static ActorSystem actorSystem;
    public static ArrayList<ActorRef> espadachines = new ArrayList<>();
    
    public static void main(String[] args) throws InterruptedException {
        actorSystem = ActorSystem.create("ActorSystem");
        dios = actorSystem.actorOf(Props.create(Dios.class), "dios");
        for(int i=0; i<CANTIDAD_ESPADACHINES; i++){
            espadachines.add(actorSystem.actorOf(Props.create(Espadachin.class), "espadachin"+i));
        }
        herrero = actorSystem.actorOf(Props.create(Herrero.class), "herrero");
        minero = actorSystem.actorOf(Props.create(Minero.class), "minero");
        dios.tell(Dios.Mensaje.INICIA, ActorRef.noSender());
    }
}
