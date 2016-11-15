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
import org.newdawn.slick.SlickException;
import org.newdawn.slick.Sound;
import org.newdawn.slick.TrueTypeFont;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;
import org.newdawn.slick.state.transition.FadeInTransition;
import org.newdawn.slick.state.transition.FadeOutTransition;

public class Play extends BasicGameState {

    private Image tiles;
    private Random rand;
    private TrueTypeFont ttf;
    private Sound scream;
    
    private World world;
    
    private long time = System.currentTimeMillis()/1000L;
    private long prev_time = time;
    private int player_x = 0;
    private int player_y = 0;
    
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
        world.load_map("omgra0");

        mapWindow = new Window(tiles, 0, 0, 9, 9);
        
    }
    
    @Override
    public void enter(GameContainer container, StateBasedGame game) throws SlickException {
    }
    
    @Override
    public void render(GameContainer container, StateBasedGame game, Graphics g) throws SlickException {
        g.setBackground(Color.black);       
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
        
        ttf.drawString(300, 8, "Mindmelt!",Color.cyan);
    }


    @Override
    public void update(GameContainer container, StateBasedGame game, int delta) throws SlickException {
        if(container.getInput().isKeyDown(Input.KEY_ESCAPE)) {
            game.enterState(Mindmelt.mainMenu,new FadeOutTransition(),new FadeInTransition());
        } else if(container.getInput().isKeyPressed(Input.KEY_SPACE)) {
            scream.play();
        } else if(container.getInput().isKeyPressed(Input.KEY_LEFT)) {
            player_x--;
        } else if(container.getInput().isKeyPressed(Input.KEY_RIGHT)) {
            player_x++;
        } else if(container.getInput().isKeyPressed(Input.KEY_UP)) {
            player_y--;
        } else if(container.getInput().isKeyPressed(Input.KEY_DOWN)) {
            player_y++;
        }
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
