// ParseTower Example - Tower Defense Game Configuration

// Define the game map
map GameMap1 {
    size = (20, 15);
    path = [(0,7), (5,7), (5,3), (10,3), (10,10), (15,10), (15,7), (19,7)];
}

// Define enemy types
enemy Goblin {
    hp = 50;
    speed = 1.5;
    reward = 10;
}

enemy Orc {
    hp = 150;
    speed = 1.0;
    reward = 25;
}

enemy Dragon {
    hp = 500;
    speed = 0.8;
    reward = 100;
}

// Define tower types
tower Arrow {
    range = 3;
    damage = 15;
    fire_rate = 2.0;
    cost = 50;
}

tower Cannon {
    range = 5;
    damage = 50;
    fire_rate = 0.5;
    cost = 150;
}

tower Magic {
    range = 4;
    damage = 30;
    fire_rate = 1.5;
    cost = 100;
}

// Define waves
wave Wave1 {
    spawn(Goblin, count=10, start=0, interval=2);
    spawn(Orc, count=3, start=20, interval=5);
}

wave Wave2 {
    spawn(Goblin, count=15, start=0, interval=1);
    spawn(Orc, count=8, start=15, interval=3);
    spawn(Dragon, count=1, start=50, interval=1);
}

wave Wave3 {
    spawn(Orc, count=20, start=0, interval=2);
    spawn(Dragon, count=5, start=30, interval=10);
}

// Place some initial towers
place Arrow at (3, 5);
place Arrow at (3, 9);
place Cannon at (8, 3);
place Magic at (12, 10);

