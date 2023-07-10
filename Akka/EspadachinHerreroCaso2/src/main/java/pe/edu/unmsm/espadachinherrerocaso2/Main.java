/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Project/Maven2/JavaApp/src/main/java/${packagePath}/${mainClassName}.java to edit this template
 */

package pe.edu.unmsm.espadachinherrerocaso2;

import pe.edu.unmsm.espadachinherrerocaso2.actores.Espadachin;

/**
 *
 * @author nicol
 */
public final class Main {

    public static void main(String[] args) {
        akka.Main.main(new String[]{Espadachin.class.getName()});
    }
}