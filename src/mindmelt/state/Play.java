package mindmelt.state;

import java.awt.Font;
import java.util.Random;
import mindmelt.Mindmelt;
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
        tiles.setFilter(Image.FILTER_NEAREST);
        rand = new Random(1);
        Font font = new Font("Monospaced",Font.PLAIN,14);
        ttf = new TrueTypeFont(font,false);
        scream = new Sound("res/wilhelm.ogg");
    }
    @Override
    public void enter(GameContainer container, StateBasedGame game) throws SlickException {
        world = new World();
        world.load_map("world");
    }
    
    @Override
    public void render(GameContainer container, StateBasedGame game, Graphics g) throws SlickException {
        g.setBackground(Color.black);
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
        for (int y=0; y<16; y++) {
            for (int x=0; x<20; x++) {
                x1 = x*32;
                y1 = y*32;
                //int tile = rand.nextInt(184)+1;
                TileType tile = world.getTile(x, y, 0);
                if (tile!= null) drawTile(x1,y1,tile.getIcon());
            }
        }
        tiles.endUse();        
        ttf.drawString(100, 100, "Mindmelt!",Color.cyan);
    }


    @Override
    public void update(GameContainer container, StateBasedGame game, int delta) throws SlickException {
        if(container.getInput().isKeyDown(Input.KEY_ESCAPE)) {
            game.enterState(Mindmelt.mainMenu,new FadeOutTransition(),new FadeInTransition());
        } else if(container.getInput().isKeyPressed(Input.KEY_SPACE)) {
            scream.play();
        }
    }   
    
    private void drawTile(int x, int y, int tile) {
        int ty = tile/20;
        int tx = tile%20;
        tiles.drawEmbedded(x,y, x+32,y+32, tx*32,ty*32, tx*32+32,ty*32+32);       
    }    
}
