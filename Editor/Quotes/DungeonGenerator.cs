using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Module {
    using Properties;

    /// <summary>
    /// ダンジョンマップのランダム生成を行うクラスです。
    /// </summary>
    public static class DungeonGenerator {
        private static readonly Random rand = new Random();
        private const int WallHeight = 2;                   //壁の高さ
        private const int MinRoomWidth = 2;                 //実質領域の最低幅
        private const int MinRoomHeight = WallHeight + 2;   //実質領域の最低高さ
        private const int MinRoomBottomMargin = 3;          //実質領域の下部余白の最低値
        private const int MinSplitWidth = 8;                //分割の最低幅
        private const int MinSplitHeight = 10;              //分割の最低高さ
        private const int MinSplitRate = 10;                //最低分割率
        private const int MaxSplitRate = 40;                //最高分割率
        private const int DirectionCount = 4;               //方向の数
        private const int ComplexMaxLevel = 5;              //複雑度の最大値

        private const int MaxRebuildCount = 1000;           //作り直しできる限界回数

        /// <summary>
        /// 生成されたタイルの情報
        /// </summary>
        public enum GeneratedMapTile {
            Floor,      //床: 通行OK
            Wall,       //壁: 通行NG
            Ceil,       //天井: 通行NG
        }

        /// <summary>
        /// 分割方向
        /// </summary>
        public enum SplitDirection {
            Horizontal,     //上下分割
            Vertical,       //左右分割
        }

        /// <summary>
        /// 通路情報: 分割線
        /// </summary>
        public class LinkData {
            public SplitDirection SplitType;        //分割方向
            public Point Start = Point.Empty;       //左端・上端
            public Point End = Point.Empty;         //右端・下端
            public int CrossCount = 0;              //この分割線に接続している通路の数

            /// <summary>
            /// 指定した座標がこの分割線の上にあるかどうかを調べる
            /// </summary>
            public bool Contains(Point pos) {
                return Rectangle.FromLTRB(this.Start.X, this.Start.Y, this.End.X + 1, this.End.Y + 1).Contains(pos);
            }
        }

        /// <summary>
        /// 小部屋情報: 分割した各エリアに一つの小部屋を作る
        /// </summary>
        public class RoomData {
            public Rectangle AreaRange;                     //小部屋理論領域
            public Rectangle RoomRange = Rectangle.Empty;   //小部屋実質領域
            public Point LinkPoint = Point.Empty;           //通路を繋ぐ交点座標
            public Point[] CrossPoints = null;              //部屋から通路を作るときに使う交点座標: 添え字は４方向
            public bool[] AllowLinks = null;                //該当方向に通路が作れるかどうか: 添え字は４方向
            public bool[] LinkExists = null;                //該当方向に通路が作られたかどうか: 添え字は４方向
            public int MaxGateCount;                        //部屋から作る通路の最大数
            public int GateCount;                           //この部屋から作られた通路の数

            /// <summary>
            /// コンストラクター
            /// </summary>
            public RoomData(Rectangle areaRange) {
                this.AreaRange = areaRange;
                this.CrossPoints = new Point[DirectionCount];
                this.AllowLinks = new bool[DirectionCount];
                this.LinkExists = new bool[DirectionCount];

                //通路用の余白をランダムに決定する
                int xWallLeftWidth;
                int xWallRightWidth;
                int yWallTopHeight;
                int yWallBottomHeight;
                do {
                    //左右の余白
                    xWallLeftWidth = rand.Next(1, (areaRange.Width - MinRoomWidth) / 2);
                    xWallRightWidth = rand.Next(1, (areaRange.Width - MinRoomWidth) / 2);

                    //上下の余白
                    yWallTopHeight = rand.Next(1, (areaRange.Height - MinRoomHeight) / 2);
                    yWallBottomHeight = MinRoomBottomMargin + rand.Next(0, (areaRange.Height - MinRoomHeight) / 2);

                    //実質領域を確定する
                    this.RoomRange = new Rectangle(
                        areaRange.X + xWallLeftWidth,
                        areaRange.Y + yWallTopHeight,
                        areaRange.Width - (xWallLeftWidth + xWallRightWidth),
                        areaRange.Height - (yWallTopHeight + yWallBottomHeight)
                    );
                } while (this.RoomRange.Height < MinRoomHeight);        //壁だけ、もしくは高さ１（床部分）の小部屋を作らないようにする
            }
        }

        /// <summary>
        /// ダンジョン全体の情報
        /// </summary>
        public class DungeonData {
            public Rectangle DungeonRect;
            public List<RoomData> Rooms = new List<RoomData>();         //小部屋リスト
            public List<LinkData> Links = new List<LinkData>();         //分割線リスト

            /// <summary>
            /// コンストラクター
            /// </summary>
            public DungeonData(Size mapSize) {
                this.DungeonRect = new Rectangle(Point.Empty, mapSize);
                this.Init();
            }

            /// <summary>
            /// このダンジョンを初期化します。
            /// </summary>
            public void Init() {
                //唯一の小部屋を持つダンジョンにする
                this.Rooms.Clear();
                this.Rooms.Add(new RoomData(new Rectangle(1, 1, this.DungeonRect.Width - 1 * 2, this.DungeonRect.Height - 1 * 2)));

                //分割線を初期化
                this.Links.Clear();

                //暫定的にすべて天井にする
                this.TileData = new GeneratedMapTile[this.DungeonRect.Width, this.DungeonRect.Height];
                for (var x = 0; x < this.DungeonRect.Width; x++) {
                    for (var y = 0; y < this.DungeonRect.Height; y++) {
                        this.TileData[x, y] = GeneratedMapTile.Ceil;
                    }
                }
            }

            /// <summary>
            /// タイルデータ
            /// </summary>
            public GeneratedMapTile[,] TileData {
                get;
                private set;
            }

            /// <summary>
            /// 分割方向をランダムに決定します。
            /// </summary>
            private static SplitDirection GetRandomSplitDirection() {
                if (rand.Next(0, 100) < 50) {
                    return SplitDirection.Horizontal;
                } else {
                    return SplitDirection.Vertical;
                }
            }

            /// <summary>
            /// 指定した小部屋を分割します。
            /// </summary>
            /// <param name="baseRoomIndex">分割する小部屋のインデックス</param>
            /// <param name="splRate">分割率 (0~100)</param>
            /// <returns>分割に成功しかどうか</returns>
            public bool RandomSplitArea(int baseRoomIndex, int splRate) {
                int splPoint;
                var newArea = new Rectangle();
                var newLink = new LinkData();

                //分割元の小部屋で辺の長い方を見て分割する: 極端に狭い部屋が生成されすぎないようにする
                if (this.Rooms[baseRoomIndex].AreaRange.Width > this.Rooms[baseRoomIndex].AreaRange.Height) {
                    //横長のときは左右分割
                    newLink.SplitType = SplitDirection.Vertical;
                } else {
                    //縦長の時は上下分割
                    newLink.SplitType = SplitDirection.Horizontal;
                }

                //与えられた小部屋を分割する
                switch (newLink.SplitType) {
                    case SplitDirection.Horizontal:
                        //上下分割: 横線

                        //分割線の設定
                        splPoint = this.Rooms[baseRoomIndex].AreaRange.Height * splRate / 100;
                        if (splPoint < MinSplitHeight) {
                            //細かすぎて分割できない場合は処理しない
                            return false;
                        }
                        newLink.Start = new Point(
                            this.Rooms[baseRoomIndex].AreaRange.X,
                            this.Rooms[baseRoomIndex].AreaRange.Y + splPoint
                        );
                        newLink.End = new Point(
                            this.Rooms[baseRoomIndex].AreaRange.X + this.Rooms[baseRoomIndex].AreaRange.Width - 1,
                            this.Rooms[baseRoomIndex].AreaRange.Y + splPoint
                        );

                        //分割して新しくできた小部屋の設定
                        newArea.Location = new Point(
                            this.Rooms[baseRoomIndex].AreaRange.X,
                            this.Rooms[baseRoomIndex].AreaRange.Y + splPoint + 1
                        );
                        newArea.Size = new Size(
                            this.Rooms[baseRoomIndex].AreaRange.Width,
                            this.Rooms[baseRoomIndex].AreaRange.Height - splPoint - 1
                        );

                        //分割元の小部屋の再設定
                        this.Rooms[baseRoomIndex] = new RoomData(new Rectangle(
                            this.Rooms[baseRoomIndex].AreaRange.Location,
                            new Size(this.Rooms[baseRoomIndex].AreaRange.Width, splPoint - 1)
                        ));
                        break;

                    case SplitDirection.Vertical:
                        //左右分割: 縦線

                        //分割線の設定
                        splPoint = this.Rooms[baseRoomIndex].AreaRange.Width * splRate / 100;
                        if (splPoint < MinSplitWidth) {
                            //細かすぎて分割できない場合は処理しない
                            return false;
                        }
                        newLink.Start = new Point(
                            this.Rooms[baseRoomIndex].AreaRange.X + splPoint,
                            this.Rooms[baseRoomIndex].AreaRange.Y
                        );
                        newLink.End = new Point(
                            this.Rooms[baseRoomIndex].AreaRange.X + splPoint,
                            this.Rooms[baseRoomIndex].AreaRange.Y + this.Rooms[baseRoomIndex].AreaRange.Height - 1
                        );

                        //分割して新しくできた小部屋の設定
                        newArea.Location = new Point(this.Rooms[baseRoomIndex].AreaRange.X + splPoint + 1, this.Rooms[baseRoomIndex].AreaRange.Y);
                        newArea.Size = new Size(this.Rooms[baseRoomIndex].AreaRange.Width - splPoint - 1, this.Rooms[baseRoomIndex].AreaRange.Height);

                        //分割元の小部屋の再設定
                        this.Rooms[baseRoomIndex] = new RoomData(new Rectangle(
                            this.Rooms[baseRoomIndex].AreaRange.Location,
                            new Size(splPoint - 1, this.Rooms[baseRoomIndex].AreaRange.Height)
                        ));
                        break;
                }

                //分割して新しくできた小部屋を追加する
                this.Rooms.Add(new RoomData(newArea));

                //分割線を追加する
                this.Links.Add(newLink);

                //分割線の部分を暫定的に床にする
                switch (newLink.SplitType) {
                    case SplitDirection.Horizontal:
                        for (var x = newLink.Start.X; x <= newLink.End.X; x++) {
                            this.TileData[x, newLink.Start.Y] = GeneratedMapTile.Floor;
                        }
                        break;

                    case SplitDirection.Vertical:
                        for (var y = newLink.Start.Y; y <= newLink.End.Y; y++) {
                            this.TileData[newLink.Start.X, y] = GeneratedMapTile.Floor;
                        }
                        break;
                }
                return true;
            }

            /// <summary>
            /// 指定した進行方向で通路を削ります。
            /// </summary>
            /// <param name="link">対象とする通路</param>
            /// <param name="trimDirection">削っていく方向 (東向きの場合は左端が削られます)</param>
            public void TrimLink(LinkData link, Map.Direction4 trimDirection) {
                //始点・終点・固定軸を決定する
                var start = 0;
                var end = 0;
                var pivot = 0;
                var step = 0;

                switch (trimDirection) {
                    case Map.Direction4.East:
                        start = link.Start.X;
                        end = link.End.X;
                        pivot = link.Start.Y;
                        step = 1;
                        break;

                    case Map.Direction4.West:
                        start = link.End.X;
                        end = link.Start.X;
                        pivot = link.Start.Y;
                        step = -1;
                        break;

                    case Map.Direction4.South:
                        start = link.Start.Y;
                        end = link.End.Y;
                        pivot = link.Start.X;
                        step = 1;
                        break;

                    case Map.Direction4.North:
                        start = link.End.Y;
                        end = link.Start.Y;
                        pivot = link.Start.X;
                        step = -1;
                        break;
                }

                //逐次削っていく
                for (var p = start; (0 < step) ? (start <= p && p <= end) : (end <= p && p <= start); p += step) {
                    var cutFlag = true;

                    //次のタイル位置をセットする
                    var checkTilePos = Point.Empty;
                    switch (trimDirection) {
                        case Map.Direction4.East:
                        case Map.Direction4.West:
                            checkTilePos = new Point(p, pivot);
                            break;

                        case Map.Direction4.South:
                        case Map.Direction4.North:
                            checkTilePos = new Point(pivot, p);
                            break;
                    }

                    //３方向チェック
                    for (var d = 0; d < DirectionCount; d++) {
                        if (d == (int) trimDirection) {
                            //進行方向はチェックしない
                            continue;
                        }

                        var checkTileNextPos = getNextDirectionPosition(checkTilePos, (Map.Direction4) d);
                        if (this.TileData[checkTileNextPos.X, checkTileNextPos.Y] == GeneratedMapTile.Floor) {
                            cutFlag = false;
                            break;
                        }
                    }

                    if (cutFlag) {
                        //現在のタイルを削る
                        this.TileData[checkTilePos.X, checkTilePos.Y] = GeneratedMapTile.Ceil;
                    }
                }
            }

            /// <summary>
            /// このダンジョンのランダム生成を実行します。
            /// </summary>
            /// <param name="complexLevel">複雑度: 分割数に関与</param>
            /// <returns>正常に生成できたかどうか</returns>
            public bool DoGenerate(int complexLevel) {
                //何回分割するかを概算する
                var fy = this.DungeonRect.Width * this.DungeonRect.Height;
                var count = 0;
                while (fy > MinRoomWidth * MinRoomHeight) {
                    fy /= 2;
                    count++;
                }

                //1. 分割線を作る
                var splitCount = count * complexLevel / ComplexMaxLevel;
                var lastIndex = 0;
                for (var n = 0; n < splitCount; n++) {
                    if (this.RandomSplitArea(lastIndex, rand.Next(MinSplitRate, MaxSplitRate))) {
                        //分割に成功したら次の分割を試行する
                        lastIndex++;
                    }
                }

                //2. 分割してできた各小部屋について、分割線に向かう通路を生成する
                foreach (var room in this.Rooms) {
                    //小部屋内で通路の起点を作る
                    room.LinkPoint = new Point(
                        room.RoomRange.X + rand.Next(0, room.RoomRange.Width),
                        room.RoomRange.Y + rand.Next(WallHeight, room.RoomRange.Height)
                    );

                    //起点から４方向に向かって分割線があるかどうかをチェックする: 範囲は分割線を含めた小部屋理論領域
                    room.MaxGateCount = 0;
                    var checkRange = new Rectangle(
                        new Point(room.AreaRange.X - 1/*境界線分*/, room.AreaRange.Y - 1/*境界線分*/),
                        new Size(room.AreaRange.Width + 1 * 2 + 1/*境界線分*/, room.AreaRange.Height + 1 * 2 + 1/*境界線分*/)
                    );
                    for (var d = 0; d < DirectionCount; d++) {
                        var nextTile = room.LinkPoint;

                        while (true) {
                            //次の座標に進む
                            nextTile = getNextDirectionPosition(nextTile, (Map.Direction4) d);
                            if (!checkRange.Contains(nextTile) || !this.DungeonRect.Contains(nextTile)) {
                                //次の座標が範囲外になったら分割線がないと判断する
                                room.AllowLinks[d] = false;
                                break;
                            } else if (this.TileData[nextTile.X, nextTile.Y] == GeneratedMapTile.Floor) {
                                //次の座標が床になっている場合は分割線であると判断する
                                room.AllowLinks[d] = true;
                                room.CrossPoints[d] = nextTile;
                                room.MaxGateCount++;
                                break;
                            }
                        }
                    }

                    //分割線に向かう通路をランダムに生成する
                    room.GateCount = rand.Next(1, room.MaxGateCount + 1);
                    var generatedGateCount = 0;
                    while (generatedGateCount < room.GateCount && room.GateCount <= room.MaxGateCount) {
                        //方向をランダムに決定する
                        var dir = rand.Next(0, DirectionCount);
                        if (!room.AllowLinks[dir]) {
                            //その方向に分割線がなければやり直し
                            continue;
                        } else if (room.LinkExists[dir]) {
                            //その方向の通路を既に作っていたらやり直し
                            continue;
                        }

                        //分割線への接続本数を更新する
                        foreach (var link in this.Links) {
                            if (link.Contains(room.CrossPoints[dir])) {
                                link.CrossCount++;
                                break;
                            }
                        }

                        //通路生成: 起点から分割線との交点までをすべて床にする
                        generatedGateCount++;
                        room.LinkExists[dir] = true;
                        switch ((Map.Direction4) dir) {
                            case Map.Direction4.East:
                                for (var x = room.LinkPoint.X; x < room.CrossPoints[dir].X; x++) {
                                    this.TileData[x, room.LinkPoint.Y] = GeneratedMapTile.Floor;
                                }
                                break;

                            case Map.Direction4.West:
                                for (var x = room.LinkPoint.X; room.CrossPoints[dir].X < x; x--) {
                                    this.TileData[x, room.LinkPoint.Y] = GeneratedMapTile.Floor;
                                }
                                break;

                            case Map.Direction4.South:
                                for (var y = room.LinkPoint.Y; y < room.CrossPoints[dir].Y; y++) {
                                    this.TileData[room.LinkPoint.X, y] = GeneratedMapTile.Floor;
                                }
                                break;

                            case Map.Direction4.North:
                                for (var y = room.LinkPoint.Y; room.CrossPoints[dir].Y < y; y--) {
                                    this.TileData[room.LinkPoint.X, y] = GeneratedMapTile.Floor;
                                }
                                break;
                        }
                    }
                }

                //3. 分割線に正しく接続できているか調べる
                foreach (var link in this.Links) {
                    if (link.CrossCount < 2) {
                        //接続本数が２本未満だと、孤立した小部屋ができてしまうのでやり直し
                        this.Init();
                        return false;
                    }
                }

                //4. 分割線の両端を削る: 分割線の検証方向以外の方向に床がなければ逐次削っていく
                foreach (var link in this.Links) {
                    switch (link.SplitType) {
                        case SplitDirection.Horizontal:
                            //上下分割: 横線
                            this.TrimLink(link, Map.Direction4.East);
                            this.TrimLink(link, Map.Direction4.West);
                            break;

                        case SplitDirection.Vertical:
                            //左右分割: 縦線
                            this.TrimLink(link, Map.Direction4.South);
                            this.TrimLink(link, Map.Direction4.North);
                            break;
                    }
                }

                //5. 各通路の上部を壁にする
                foreach (var link in this.Links) {
                    switch (link.SplitType) {
                        case SplitDirection.Horizontal:
                            //横線は上部がすべて壁になる

                            for (var x = link.Start.X; x <= link.End.X; x++) {
                                if (this.TileData[x, link.Start.Y] != GeneratedMapTile.Floor) {
                                    //削られた通路部分は処理しない
                                    continue;
                                }

                                //壁にしようとしている部分が既に床にされていないか検証する
                                var wallNG = false;
                                for (var n = 1; n <= WallHeight; n++) {
                                    if (this.TileData[x, link.Start.Y - n] == GeneratedMapTile.Floor) {
                                        wallNG = true;
                                        break;
                                    }
                                }
                                if (wallNG) {
                                    //既に床にされているときは塞がない
                                    continue;
                                }

                                //壁化実行
                                for (var n = 1; n <= WallHeight; n++) {
                                    this.TileData[x, link.Start.Y - n] = GeneratedMapTile.Wall;
                                }
                            }
                            break;

                        case SplitDirection.Vertical:
                            //縦線は一番上だけを壁にする

                            for (var y = link.Start.Y; y <= link.End.Y; y++) {
                                if (this.TileData[link.Start.X, y] != GeneratedMapTile.Floor) {
                                    //削られた通路部分は処理しない
                                    continue;
                                }

                                //壁にしようとしている部分が既に床にされていないか検証する
                                var wallNG = false;
                                for (var n = 1; n <= WallHeight; n++) {
                                    if (this.TileData[link.Start.X, y - n] == GeneratedMapTile.Floor) {
                                        wallNG = true;
                                        break;
                                    }
                                }
                                if (wallNG) {
                                    //既に床にされているときは塞がない
                                    break;
                                }

                                //壁化実行
                                for (var n = 1; n <= WallHeight; n++) {
                                    this.TileData[link.Start.X, y - n] = GeneratedMapTile.Wall;
                                }
                                break;
                            }
                            break;
                    }
                }

                //6. 各小部屋の床と壁を作る
                foreach (var room in this.Rooms) {
                    //6-1. 各小部屋内部の床と壁を作る
                    for (var x = room.RoomRange.Left; x < room.RoomRange.Right; x++) {
                        for (int y = room.RoomRange.Top, n = 0; y < room.RoomRange.Bottom; y++, n++) {
                            if (n < WallHeight
                            && (room.LinkExists[(int) Map.Direction4.North] == false
                            || room.LinkPoint.X != x)) {
                                //上２マスは壁にする: ただし通路は塞がない
                                this.TileData[x, y] = GeneratedMapTile.Wall;
                            } else {
                                this.TileData[x, y] = GeneratedMapTile.Floor;
                            }
                        }
                    }

                    //6-2. 横線の通路に上部に壁を作る
                    for (var d = 0; d < DirectionCount; d++) {
                        if (!room.LinkExists[d]) {
                            continue;      //通路がない場合は処理しない
                        }
                        switch ((Map.Direction4) d) {
                            case Map.Direction4.East:
                                for (var x = room.RoomRange.Right; x <= room.AreaRange.Right; x++) {
                                    for (var n = 1; n <= WallHeight; n++) {
                                        this.TileData[x, room.LinkPoint.Y - n] = GeneratedMapTile.Wall;
                                    }
                                }
                                break;

                            case Map.Direction4.West:
                                for (var x = room.AreaRange.Left; x < room.RoomRange.Left; x++) {
                                    for (var n = 1; n <= WallHeight; n++) {
                                        this.TileData[x, room.LinkPoint.Y - n] = GeneratedMapTile.Wall;
                                    }
                                }
                                break;
                        }
                    }
                }

                //正常に生成された
                return true;
            }

            /// <summary>
            /// 現在のタイルデータを文字列に変換する
            /// </summary>
            public string TileDataToString() {
                var buf = "";

                //一行ごとに生成する
                for (var y = 0; y < this.DungeonRect.Height; y++) {
                    for (var x = 0; x < this.DungeonRect.Width; x++) {
                        switch (this.TileData[x, y]) {
                            case GeneratedMapTile.Floor:
                                buf += "　";
                                break;
                            case GeneratedMapTile.Wall:
                                buf += "壁";
                                break;
                            case GeneratedMapTile.Ceil:
                                buf += "■";
                                break;
                        }
                    }
                    buf += "\r\n";
                }

                return buf;
            }
        }

        /// <summary>
        /// 起点から指定した方向に一つ進んだ座標を返します。
        /// </summary>
        private static Point getNextDirectionPosition(Point startPos, Map.Direction4 dir) {
            var nextTile = startPos;

            switch (dir) {
                case Map.Direction4.East:
                    nextTile = new Point(nextTile.X + 1, nextTile.Y);
                    break;
                case Map.Direction4.South:
                    nextTile = new Point(nextTile.X, nextTile.Y + 1);
                    break;
                case Map.Direction4.West:
                    nextTile = new Point(nextTile.X - 1, nextTile.Y);
                    break;
                case Map.Direction4.North:
                    nextTile = new Point(nextTile.X, nextTile.Y - 1);
                    break;
            }

            return nextTile;
        }

        /// <summary>
        /// 指定したサイズに収まるランダムダンジョンを生成します。
        /// </summary>
        public static DungeonData DoGenerateDungeon(Size mapSize, int complexLevel) {
            int i;
            var map = new DungeonData(mapSize);

            for (i = 1; map.DoGenerate(complexLevel) == false && i <= MaxRebuildCount; i++) {
                //正常に生成されるまで繰り返す
                System.Diagnostics.Debug.WriteLine("ダンジョンの生成に失敗しました。再試行します... [" + i.ToString() + " 回目]");
            }

            if (i > MaxRebuildCount) {
                //作り直しの限界数に達した場合は中止する
                MessageBox.Show("ランダム生成中に何らかの問題があったため、処理を中断します。\r\n生成オプションを確認して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return null;
            }

            return map;
        }
    }
}
