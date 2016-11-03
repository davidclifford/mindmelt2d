/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mindmelt;

import java.util.Random;
import org.newdawn.slick.state.*;
import org.newdawn.slick.font.*;
import org.newdawn.slick.*;
import java.awt.Font;
        
public class MainMenu extends BasicGameState {

    private Image tiles;
    private Random rand;
    private TrueTypeFont ttf;

    @Override
    public int getID() {
        return Mindmelt.mainMenu;
    }

    @Override
    public void init(GameContainer gc, StateBasedGame state) throws SlickException {
        tiles = new Image("res/tilesx2.bmp",false,0, Color.black);
        tiles.setFilter(Image.FILTER_NEAREST);
        rand = new Random();
        Font font = new Font("Courier New",Font.PLAIN,14);
        ttf = new TrueTypeFont(font,false);
    }

    @Override
    public void update(GameContainer gc, StateBasedGame state, int ticks) throws SlickException {
        if(gc.getInput().isKeyPressed(Input.KEY_ESCAPE)) {
            gc.exit();
        }
    }

    @Override
    public void render(GameContainer gc, StateBasedGame state, Graphics g) throws SlickException {
        ttf.drawString(100, 100, "Mindmelt!",Color.cyan);
        int x1 = rand.nextInt(640);
        int y1 = rand.nextInt(480);
        for (int i=0; i<100; i++) {
            int x2 = rand.nextInt(640);
            int y2 = rand.nextInt(480);
            int red = rand.nextInt(255);
            int green = rand.nextInt(255);
            int blue = rand.nextInt(255);
            Color c = new Color(red, green, blue);
            g.setColor(c);
            g.drawLine(x1, y1, x2, y2);
            x1 = x2;
            y1 = y2;
        }
        
        tiles.startUse();
        for (int i=0; i<222; i++) {
            x1 = rand.nextInt(30)*32;
            y1 = rand.nextInt(20)*32;
            int tile = rand.nextInt(184)+1;
            drawTile(x1,y1,tile);
        }
        tiles.endUse();
    }
    
    private void drawTile(int x, int y, int tile) {
        int tx = tile/20;
        int ty = tile%20;
        tiles.drawEmbedded(x,y, x+32,y+32, tx*32,ty*32, tx*32+32,ty*32+32);       
    }
}
