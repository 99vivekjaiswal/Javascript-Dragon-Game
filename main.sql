CREATE TABLE Players (
    PlayerID INT AUTO_INCREMENT PRIMARY KEY,
    Username VARCHAR(50) NOT NULL,
    HighScore INT DEFAULT 0,
    LastLogin DATE
);
