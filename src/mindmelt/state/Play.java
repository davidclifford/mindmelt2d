package mindmelt.state;

import java.awt.Font;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import mindmelt.Mindmelt;
import mindmelt.action.Action;
import mindmelt.action.ZoneAdjacentAction;
import mindmelt.engine.Engine;
import mindmelt.gui.Window;
import mindmelt.gui.WindowSystem;
import mindmelt.gui.Zone;
import mindmelt.maps.EntryExit;
import mindmelt.maps.World;
import mindmelt.objects.Obj;
import mindmelt.objects.ObjectStore;
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
    private TrueTypeFont guiFont;
    private Sound scream;
    
    private World world;
    private ObjectStore objects;
    private Engine engine;
    
    private int player_x = 0;
    private int player_y = 0;
    private int player_z = 0;
    private int dir = 0;
    private int mapId = 1;
    private Obj player;
    
    private int mouseIcon = 182;
    private String keypress = "";
    private boolean up;
    private boolean down;
    private boolean left;
    private boolean right;
    private boolean pressed = false;
    private String mouseString = "";
    
    private WindowSystem windowSystem;
    private Window mapWindow;
    private Window invWindow;
    
    int size = 9;
    int half = size/2;
    int tile[][];
    List<DispXY> dispList;
    boolean xray = false;
    boolean light = false;
    boolean see_all = true;
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
    
    class DispXYString {
        public int x;
        public int y;
        public String string;
        DispXYString(int x, int y, String string) {
            this.x = x;
            this.y = y;
            this.string = string;
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
        tiles = new Image("res/tiles.png",false,0,new Color(132, 0, 0)); 
        tiles.setFilter(Image.FILTER_LINEAR);
        rand = new Random();
        Font font = new Font("Monospaced",Font.BOLD,24);
        ttf = new TrueTypeFont(font,false);
        Font font2 = new Font("Monospaced",Font.BOLD,12);
        guiFont = new TrueTypeFont(font2,false);
        scream = new Sound("res/wilhelm.ogg");
           
        world = new World();
        world.loadMap("world");

        windowSystem = new WindowSystem();
        mapWindow = new Window(tiles, ttf, 1, 1, 10, 10);
        windowSystem.addWindow(mapWindow);
        invWindow = new Window(tiles, ttf, 1,12, 10,1);
        windowSystem.addWindow(invWindow);
        
        Image mouse = mapWindow.getTile(tiles, mouseIcon);
        container.setMouseCursor(mouse,16, 16);

        objects = new ObjectStore();
        objects.loadObjects("initial");
        
        objects.initMap(world);
        player = objects.getPlayer();
        player.setSpeed(150);
        player_x = player.getX();
        player_y = player.getY();
        player_z = player.getZ();
               
        engine = new Engine(world, objects);
        
        Zone zoneAroundPlayer = new Zone(half,half,3,3);
        zoneAroundPlayer.setAction(new ZoneAdjacentAction(engine));
        mapWindow.addZone(zoneAroundPlayer);
        
        disp_init();
    }
    
    @Override
    public void enter(GameContainer container, StateBasedGame game) throws SlickException {
        container.getInput().clearKeyPressedRecord();
        container.getInput().clearMousePressedRecord();
        container.getInput().clearControlPressedRecord();
        container.getInput().disableKeyRepeat();
    }
    
    @Override
    public void render(GameContainer container, StateBasedGame game, Graphics g) throws SlickException {
        world = engine.getWorld();
        g.setBackground(Color.black);    
        display_position(player_x,player_y, dir);
        display_inventory(player);
        
        guiFont.drawString(480, 0, "Mindmelt!",Color.cyan);
        guiFont.drawString(480, 16, keypress, Color.yellow);
        guiFont.drawString(480, 32, "X: "+player_x+" Y:"+player_y+" Tile: "+world.getTile(player_x+4, player_y+4, 0).getId(), Color.white);
        guiFont.drawString(480, 48, (xray ? "X" : "x") + (light ? "L" : "l") + (see_all ? "A" : "a") + (cheat ? "C" : "c") );
        guiFont.drawString(480, 64, world.getDescription(), Color.red );
        guiFont.drawString(480, 80, mouseString, Color.yellow );
    }
    
    private void display_inventory(Obj who) {
        List<Obj> objects = who.getObjects();
        if (objects==null || objects.isEmpty()) return;
        tiles.startUse();
        int x = 0;
        for(Obj ob : objects) {
            int icon = ob.getIcon();
            System.out.println("Inv: icon="+icon+" x="+x);
            invWindow.drawTile(tiles, x++, 1, icon);
        }
        tiles.endUse();
    }
    
    private void display_position(int px, int py, int dir) {
        int mask[][] = new int[size][size]; //0 = see & thru, 1 = see & not thru, 2 = not see & not thru
        List<Obj> obj[][] = new ArrayList[size][size];
        Obj top[][] = new Obj[size][size];
        boolean dark = !world.getLight() && !light;
        
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

        // Draw landscape
        List<DispXYString> xyStrings = new ArrayList<>();
        tiles.startUse();
        for (int y=-half;y<=half;y++) {
            for (int x=-half;x<=half;x++) {
                int tile = world.getTile(px+x, py+y, 0).icon;
                int xx = dir==0 ? x : dir==1 ? y : dir==2 ? -x : -y; 
                int yy = dir==0 ? y : dir==1 ? -x : dir==2 ? -y : x; 
                if (mask[x+half][y+half] < 2 || see_all) {
                    mapWindow.drawTile(tiles, half+xx, half+yy, tile);
                    List<Obj> objects = world.getObjects(px+x, py+y, 0);
                    if (objects!=null) {
                        for (Obj ob : objects) {
                            mapWindow.drawTile(tiles, half+xx, half+yy, ob.getIcon());
                            DispXYString xys = new DispXYString( half+xx, half+yy,ob.getMessage());
                            xyStrings.add(xys);
                        }
                    }
                } else {
                    mapWindow.drawTile(tiles, half+xx, half+yy, 0);
                }
            }
        }
        tiles.endUse();    
        
        //Display any strings
        for(DispXYString xys:xyStrings ) {
            mapWindow.drawString(xys.x, xys.y, xys.string);
        }
    }
    
    @Override
    public void update(GameContainer container, StateBasedGame game, int delta) throws SlickException {
        player_x = player.getX();
        player_y = player.getY();
        player_z = player.getZ();

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
        if(container.getInput().isKeyPressed(Input.KEY_F1)) {
            xray = !xray;
        }
        if(container.getInput().isKeyPressed(Input.KEY_F2)) {
            light = !light;
        }        
        if(container.getInput().isKeyPressed(Input.KEY_F3)) {
            see_all = !see_all;
        }            
        if(container.getInput().isKeyPressed(Input.KEY_F4)) {
            cheat = !cheat;
        }   
        if(container.getInput().isKeyPressed(Input.KEY_BACK)) {
            if(player.hasObjectInside()) {
                Obj ob = player.getObjects().get(0);
                engine.moveObjToMap(ob, player.getX(),player.getY(),player.getZ());
                engine.moveObjToMap(player, player.getX(),player.getY(),player.getZ());
            }
        }   
        if(container.getInput().isKeyPressed(Input.KEY_E)) {
            if (engine.isAnEntryExit(player_x, player_y, player_z)){
                engine.doEntryExit(player_x, player_y, player_z);
            }
        }   

        if (pressed && player.isReady(delta)) {
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

                boolean canEnter = engine.canEnter(player, new_x, new_y, 0);
                if (canEnter || cheat) {
                    player_x = new_x;
                    player_y = new_y;
                    engine.moveObjToMap(player,new_x, new_y, player_z);
                } else {
                    engine.activateTile(new_x, new_y, player_z);
                }
        }
        if (!pressed)
            player.setWait(player.getSpeed());
        player.setDirection(dir);
        updateAllObjects(delta);
    }   
    
    @Override
    public void keyPressed(int key, char c) {
        keypress = "DN Key: "+key+" Char: "+c;
        if(key == Input.KEY_UP) up = true;
        if(key == Input.KEY_DOWN) down = true;
        if(key == Input.KEY_LEFT) left = true;
        if(key == Input.KEY_RIGHT) right = true;
        if(key == Input.KEY_PRIOR) dir--;
        if(key == Input.KEY_NEXT) dir++;
        dir = dir<0 ? dir+=4 : dir>3 ? dir-=4 : dir;
        pressed = (up||down||left||right);
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
        
        mouseString = "B: "+button+" x: "+x/32+" y: "+y/32+" c: "+count;
        windowSystem.click(x, y, button);
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
    
    private void updateAllObjects(int delta) {
        for(Obj ob : objects.getActiveObjects()) {
            ob.update(engine,delta);
        }
    }
}
