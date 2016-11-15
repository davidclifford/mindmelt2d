package mindmelt;

import mindmelt.state.Play;
import mindmelt.state.MainMenu;
import java.awt.Cursor;
import java.util.Random;
import mindmelt.gui.WindowSystem;
import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.state.StateBasedGame;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;

public class Mindmelt extends StateBasedGame {
    
    public static final int mainMenu = 0;
    public static final int playScreen = 1;

    public static AppGameContainer app;
    public Image tiles;
    public WindowSystem ws;
    
    public static void main(String[] args) throws SlickException {
        app = new AppGameContainer(new Mindmelt("Mindmelt"));
        app.setDisplayMode(640, 480, false);
        app.start();
    }

    public Mindmelt(String title) {
        super(title);
        addState(new MainMenu());
        addState(new Play());
//        addState(new MiniMap());
    }

    @Override
    public void initStatesList(GameContainer container) throws SlickException {
        getState(mainMenu).init(container, this);
        enterState(mainMenu);
    }
 }
