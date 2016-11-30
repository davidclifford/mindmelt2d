package mindmelt.state;

import java.awt.Font;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import mindmelt.Mindmelt;
import mindmelt.gui.Window;
import mindmelt.gui.WindowSystem;
import mindmelt.maps.TileType;
import mindmelt.maps.World;
import mindmelt.objects.Obj;
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
    private List<Obj> objects; 
    
    private int player_x = 39;
    private int player_y = 38;
    private int dir = 0;
    private int mapId = 0;
    private Obj player = null;
    
    private int frame = 0;
    private String keypress = "";
    private boolean up;
    private boolean down;
    private boolean left;
    private boolean right;
    private boolean pressed = false;
    
    private Window mapWindow;
    
    int size = 15;
    int half = size/2;
    int tile[][];
    List<DispXY> dispList;
    boolean xray = false;
    boolean dark = false;
    boolean see_all = false;
    boolean cheat = true;
    
    class DispXY {
        public int xf;
        public int yf;
        public int xt;
        public int yt;
        
        DispXY(int x1, int y1, int x2, int y2) {
            xf=x1;
            yf=y1;
            xt=x2;
            yt=y2;
        }
    }
   

    private void disp_init() {
        dispList = new ArrayList<>();
        for(int i=1; i<=half; i++) { //distance out
            for(int j=-i;j<=i;j++) { //go
                int k = sgn(j);
                int l = sgn(i);
                dispList.add(new DispXY(j, -i, j-k, l-i)); //across right top
                dispList.add(new DispXY(j, i, j-k, i-l)); //across left bottom
                dispList.add(new DispXY(i, j, i-l, j-k)); //right down
                dispList.add(new DispXY(-i, j, l-i, j-k)); // left up
            }
        }
    }    
  
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
        
        objects = new ArrayList<Obj>();
        player = Obj.builder().id(1).name("player").description("Argmo").type("player").setCoords(player_x, player_y, 0).icon(61);
        Obj scisors = Obj.builder().id(2).name("scisors").description("a pair of scisors").type("thing").setCoords(40, 40, 0).icon(178);
        Obj gremlin = Obj.builder().id(3).name("gremlin").description("Gremlin").type("monster").setCoords(38, 36, 0).icon(62);
        objects.add(player);
        objects.add(scisors);
        objects.add(gremlin);
        Collections.reverse(objects);
        disp_init();
    }
    
    @Override
    public void enter(GameContainer container, StateBasedGame game) throws SlickException {
    }
    
    @Override
    public void render(GameContainer container, StateBasedGame game, Graphics g) throws SlickException {
        g.setBackground(Color.black);    
        display_position(player_x,player_y, dir);
        
        ttf.drawString(300, 0, "Mindmelt!",Color.cyan);
        ttf.drawString(300, 16, keypress, Color.yellow);
        ttf.drawString(300, 32, "X: "+player_x+" Y:"+player_y+" Tile: "+world.getTile(player_x+4, player_y+4, 0).getId(), Color.white);
    }
    
    private void display_pos(int px, int py) {
        int size = 15;
        int half = size/2;
        int tile;        
        tiles.startUse();
        for (int y=-half;y<half+1;y++) {
            for (int x=-half;x<half+1;x++) {
                tile = world.getTile(px+x, py+y, 0).getIcon();
                mapWindow.drawTile(tiles, x+half, y+half, tile);
            }
        }
        mapWindow.drawTile(tiles,half,half,61); //player
        tiles.endUse();    
    }    
 
    private void display_position(int px, int py, int dir) {
        int mask[][] = new int[size][size]; //0 = see & thru, 1 = see & not thru, 2 = not see & not thru
        List<Obj> obj[][] = new ArrayList[size][size];
        
        for (DispXY xy : dispList) {
            boolean canSee = world.getTile(px+xy.xf, py+xy.yf, 0).isSeeThru();
            if (mask[xy.xt+half][xy.yt+half]==0 && !canSee) {
                mask[xy.xf+half][xy.yf+half] = 1;
            } else if (mask[xy.xt+half][xy.yt+half]>=1) {
                mask[xy.xf+half][xy.yf+half] = 2;
            }
            if(xy.xf <= 1 && xy.xf >= -1 && xy.yf <= 1 && xy.yf >= -1 && xray) {
                mask[xy.xf+half][xy.yf+half] = 0;
            }
            if((xy.xf > 1 || xy.xf < -1 || xy.yf > 1 || xy.yf < -1) && dark) {
                mask[xy.xf+half][xy.yf+half] = 2;
            }
        }
        
        //Objects
        for (Obj ob : objects) {
            if (ob.mapId != mapId )
                continue;
            int x = ob.x-px;
            int y = ob.y-py;
            if (x>-half && x<half && y>-half && y<half) {
                if (obj[x+half][y+half]==null)
                    obj[x+half][y+half] = new ArrayList<Obj>();
                obj[x+half][y+half].add(ob);
            }
        }

        // Draw landscape
        tiles.startUse();
        for (int y=-half;y<=half;y++) {
            for (int x=-half;x<=half;x++) {
                int tile = world.getTile(px+x, py+y, 0).icon;
                int xx = dir==0 ? x : dir==1 ? y : dir==2 ? -x : -y; 
                int yy = dir==0 ? y : dir==1 ? -x : dir==2 ? -y : x; 
                if (mask[x+half][y+half] < 2 || see_all) {
                    mapWindow.drawTile(tiles, half+xx, half+yy, tile);
                    List<Obj> objs = obj[x+half][y+half];
                    if (objs!=null) {
                        for(Obj ob : objs)
                           mapWindow.drawTile(tiles,half+xx, half+yy,ob.icon); 
                    }
                } else {
                    mapWindow.drawTile(tiles, half+xx, half+yy, 0);
                }
            }
        }
        tiles.endUse();    
    }
    
    @Override
    public void update(GameContainer container, StateBasedGame game, int delta) throws SlickException {
        
        final int speed = 100;
        int new_x=player_x;
        int new_y=player_y;
        int a[] = {1,0,-1,0};        
        int b[] = {0,-1,0,1};        
        
        if(container.getInput().isKeyDown(Input.KEY_ESCAPE)) {
            game.enterState(Mindmelt.mainMenu,new FadeOutTransition(),new FadeInTransition());
        }        
        if(container.getInput().isKeyPressed(Input.KEY_SPACE)) {
            scream.play();
        }
        if (pressed && frame==0) {
                if (up) {
                    new_x = player_x - b[dir];
                    new_y = player_y - a[dir];                    
                }
                if (down) {
                    new_x = player_x + b[dir];
                    new_y = player_y + a[dir];                    
                }
                if (left) {
                    new_x = player_x - a[dir];
                    new_y = player_y + b[dir];                    
                }
                if (right) {
                    new_x = player_x + a[dir];
                    new_y = player_y - b[dir];
                }
                frame -= speed;

                TileType nextTile = world.getTile(new_x, new_y, 0);
                if (nextTile.canEnter || cheat) {
                    player_x = new_x;
                    player_y = new_y;
                    player.setCoords(new_x, new_y, 0);
                }
                frame = delta;
        } else if (pressed) {
            frame+=delta;
            if (frame>speed) frame = 0;
        } else {
            frame = 0;
        }
    }   
    
    @Override
    public void keyPressed(int key, char c) {
        keypress = "DOWN Key: "+key+" Char: "+c;
        if(key == Input.KEY_UP) up = true;
        if(key == Input.KEY_DOWN) down = true;
        if(key == Input.KEY_LEFT) left = true;
        if(key == Input.KEY_RIGHT) right = true;
        if(key == Input.KEY_PRIOR) dir--;
        if(key == Input.KEY_NEXT) dir++;
        dir = dir<0 ? dir+=4 : dir>3 ? dir-=4 : dir;
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
    
    private int sgn(int s) {
        return s<0 ? -1 : (s>0 ? 1 : 0);
    }
}
