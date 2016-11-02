package mindmelt;

import java.awt.Cursor;
import java.util.Random;
import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.BasicGame;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;

public class Mindmelt extends BasicGame {

    private Random rand;
    private static AppGameContainer app;
    private Image tiles;
    
    public static void main(String[] args) throws SlickException {
        app = new AppGameContainer(new Mindmelt("Mindmelt"));
        app.setDisplayMode(640, 480, false);
        app.start();
    }

    public Mindmelt(String title) {
        super(title);
    }

    @Override
    public void init(GameContainer gc) throws SlickException {
        tiles = new Image("res/tilesx2.bmp",false,0, Color.black);
        tiles.setFilter(Image.FILTER_NEAREST);
        rand = new Random();
    }

    @Override
    public void update(GameContainer gc, int ticks) throws SlickException {
        if(gc.getInput().isKeyPressed(Input.KEY_ESCAPE)) {
            app.exit();
        }
    }

    @Override
    public void render(GameContainer gc, Graphics g) throws SlickException {
        g.scale(1,1);
        g.setColor(Color.white);
        g.drawString("Mindmelt!", 100, 100);
        g.setColor(Color.red);
        tiles.startUse();
        for (int i=0; i<100000; i++) {
            int x1 = rand.nextInt(30)*32;
            int y1 = rand.nextInt(20)*32;
            int x2 = rand.nextInt(30)*32;
            int y2 = rand.nextInt(20)*32;
            int tile = rand.nextInt(184)+1;
            //g.drawLine(x1, y1, x2, y2);
            drawTile(x1,x2,tile);
        }
        tiles.endUse();
    }
    
    public void drawTile(int x, int y, int tile) {
        int tx = tile/20;
        int ty = tile%20;
        tiles.drawEmbedded(x,y, x+32,y+32, tx*32,ty*32, tx*32+32,ty*32+32);       
    }
}
