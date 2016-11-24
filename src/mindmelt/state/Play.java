package mindmelt.state;

import java.awt.Font;
import java.util.Random;
import mindmelt.Mindmelt;
import mindmelt.gui.Window;
import mindmelt.gui.WindowSystem;
import mindmelt.maps.TileType;
import mindmelt.maps.World;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.InputListener;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.Sound;
import org.newdawn.slick.TrueTypeFont;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;
import org.newdawn.slick.state.transition.FadeInTransition;
import org.newdawn.slick.state.transition.FadeOutTransition;

public class Play extends BasicGameState implements InputListener {

    private Image tiles;
    private Random rand;
    private TrueTypeFont ttf;
    private Sound scream;
    
    private World world;
    
    private int player_x = 39;
    private int player_y = 38;
    
    private int frame = 0;
    private String keypress = "";
    private boolean up;
    private boolean down;
    private boolean left;
    private boolean right;
    private boolean pressed = false;
    
    private Window mapWindow;
    
    @Override
    public int getID() {
        return Mindmelt.playScreen;
    }

    @Override
    public void init(GameContainer container, StateBasedGame game) throws SlickException {
        container.getInput().clearKeyPressedRecord();
        container.getInput().clearMousePressedRecord();
        container.getInput().clearControlPressedRecord();
        container.getInput().disableKeyRepeat();

        tiles = new Image("res/tiles.png",false,0,new Color(132, 0, 0)); 
        tiles.setFilter(Image.FILTER_LINEAR);
        rand = new Random(1);
        Font font = new Font("Monospaced",Font.PLAIN,14);
        ttf = new TrueTypeFont(font,false);
        scream = new Sound("res/wilhelm.ogg");
    
        world = new World();
        world.load_map("worldx80");

        mapWindow = new Window(tiles, 0, 0, 9, 9);
        
    }
    
    @Override
    public void enter(GameContainer container, StateBasedGame game) throws SlickException {
    }
    
    @Override
    public void render(GameContainer container, StateBasedGame game, Graphics g) throws SlickException {
        g.setBackground(Color.black);    
        display_position(player_x,player_y);
        
        ttf.drawString(300, 0, "Mindmelt!",Color.cyan);
        ttf.drawString(300, 16, keypress, Color.yellow);
        ttf.drawString(300, 32, "X: "+player_x+" Y:"+player_y+" Tile: "+world.getTile(player_x+4, player_y+4, 0).getId(), Color.white);
    }

    private void display_pos(int px, int py) {
        tiles.startUse();
        for (int y=0; y<9; y++) {
            for (int x=0; x<9; x++) {
                //int tile = rand.nextInt(184)+1;
                TileType tile = world.getTile(x+player_x, y+player_y, 0);
                //if (tile!= null) drawTile(x1,y1,tile.getIcon());
                mapWindow.drawTile(tiles, x, y, tile.getIcon());
            }
        }
        mapWindow.drawTile(tiles,4,4,61);
        tiles.endUse();    
    }
    
    private void display_position(int px, int py) {
        int size = 9;
        int half = size/2;
        int tile; 
        
        tiles.startUse();
        for (int y=-half;y<half+1;y++) {
            for (int x=-half;x<half+1;x++) {
                if ((x<2 && x>-2 && y<2 && y>-2) || clear(px+x,py+y,px,py)) {
                    tile = world.getTile(px+x, py+y, 0).getIcon();
                } else {
                    tile = TileType.space.getIcon();
                }
                mapWindow.drawTile(tiles, x+half, y+half, tile);
            }
        }
        mapWindow.drawTile(tiles,half,half,61); //player
        tiles.endUse();    
    }
    
    private boolean clear(int x1, int y1, int x2, int y2) {
        int xa = Math.abs(x1-x2);
        int ya = Math.abs(y1-y2);
        int xs = x1<x2 ? 1 : (x1>x2 ? -1 : 0);
        int ys = y1<y2 ? 1 : (y1>y2 ? -1 : 0);
        System.out.println("x1 ="+x1+" y1="+y1);
        System.out.println("x2 ="+x2+" y2="+y2);
        if (xa>ya) {
            int d = 2*xa-ya;
            int y = y1;
            for (int i=0;i<=xa;i++){
                int x=x1+i*xs;
                System.out.println("x ="+x+" y="+y);
                if (false&&!world.getTile(x, y, 0).isSeeThru())
                    return false;
                if (d>0) {
                    y+=ys;
                    d -= xa;
                }
                d += ya;
            }
        } else {
            int d = 2*ya-xa;
            int x = x1;
            for (int i=0;i<=ya;i++){
                int y=y1+i*ys;
                System.out.println("x ="+x+" y="+y);
                if (false&&!world.getTile(x, y, 0).isSeeThru())
                    return false;
                if (d>0) {
                    x+=xs;
                    d -= ya;
                }
                d += xa;
            }
        }
        return true;
    }
    
    @Override
    public void update(GameContainer container, StateBasedGame game, int delta) throws SlickException {
        
        final int speed = 100;
        int new_x=player_x;
        int new_y=player_y;
        
        if(container.getInput().isKeyDown(Input.KEY_ESCAPE)) {
            game.enterState(Mindmelt.mainMenu,new FadeOutTransition(),new FadeInTransition());
        }        
        if(container.getInput().isKeyPressed(Input.KEY_SPACE)) {
            scream.play();
        }
        if (pressed) {
            frame+=delta;
            if (frame > speed){
                if (up) new_y = player_y-1;
                if (down) new_y = player_y+1;
                if (left) new_x = player_x-1;
                if (right) new_x = player_x+1;
                frame -= speed;

                TileType nextTile = world.getTile(new_x, new_y, 0);
                if (nextTile.canEnter) {
                    player_x = new_x;
                    player_y = new_y;
                }
            }
        }
    }   
    
    @Override
    public void keyPressed(int key, char c) {
        keypress = "DOWN Key: "+key+" Char: "+c;
        if(key == Input.KEY_UP) up = true;
        if(key == Input.KEY_DOWN) down = true;
        if(key == Input.KEY_LEFT) left = true;
        if(key == Input.KEY_RIGHT) right = true;
        pressed = true;
        frame = 100;
    }
    
    @Override
    public void keyReleased(int key, char c) {
        keypress = "UP Key: "+key+" Char: "+c;
        if(key == Input.KEY_UP) up = false;
        if(key == Input.KEY_DOWN) down = false;
        if(key == Input.KEY_LEFT) left = false;
        if(key == Input.KEY_RIGHT) right = false;        
        pressed = false;
    }
    
    @Override
    public void mouseClicked(int button, int x, int y, int count) {
        System.out.println("Button: "+button+" x: "+x+" y: "+y+" count: "+count);
    }
    
    private void drawTile(int x, int y, int tile) {
        int ty = tile/20;
        int tx = tile%20;
        tiles.drawEmbedded(x,y, x+32,y+32, tx*32,ty*32, tx*32+32,ty*32+32);       
    }     
    
    private void drawSmallTile(int x, int y, int tile) {
        int ty = tile/20;
        int tx = tile%20;
        tiles.drawEmbedded(x,y, x+8,y+8, tx*32,ty*32, tx*32+32,ty*32+32);       
    }    
}
