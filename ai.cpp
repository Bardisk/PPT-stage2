#include "ai.h"

const int dx[] = {1, -1, 0, 0};
const int dy[] = {0, 0, 1, -1};
const DirectionType DD[] = {DirectionType::DW, DirectionType::UP, DirectionType::RT, DirectionType::LF};

DirectionType Ai::Decision(GameMainMap *coreData){
    Player me(coreData->plaMem(myNum)->toMap());

    if(me.isMoving && ((isthereWave(me.posTo, coreData) && mode != FLEE) || me.isBlocked(coreData)))
        me.stop(coreData);


    if (me.isMoving && me.stat > 5000)
        return me.direction();
    QQueue<loca> q;
    q.push_back(me.pos);
    memset(map, 0xff, sizeof map);
    memset(vis, 0, sizeof vis);
    map[me.pos.x][me.pos.y] = 1;
    qDebug() << "my num:" << myNum << "mode:" << mode;
    while (!q.empty()) {
        loca now = q.front();
        vis[now.x][now.y] = true;
        q.pop_front();
        QList<QVariant>::Iterator it;
        auto &nowMem = coreData->mapMem(now);
        findo(it, HARDWALL, nowMem){
            map[now.x][now.y] = -1;
            continue;
        }
        findo(it, BOMB, nowMem){
            continue;
        }
        findo(it, SOFTWALL, nowMem) continue;
        for (int i = 0; i < 4; i++){
            if (vis[now.x + dx[i]][now.y + dy[i]]) continue;
            if (map[now.x + dx[i]][now.y + dy[i]] < map[now.x][now.y] + 1 || map[now.x + dx[i]][now.y + dy[i]] == -1){
                pre[now.x + dx[i]][now.y + dy[i]] = now;
                map[now.x + dx[i]][now.y + dy[i]] = map[now.x][now.y] + 1;
                q.push_back(loca(now.x + dx[i], now.y + dy[i]));
            }
        }
    }
    for (int i = 0; i < coreData->szN; i++){
        QString deString;
        for (int j = 0; j < coreData -> szM; j++)
            deString.append(QString("%1 ").arg(map[i][j]));
        qDebug() << deString;
    }
    if (coreData->mapMem(me.pos).findElement(BOMB)){
        mode = FLEE;
        for(int k = 0; k < 4; k++){
            loca tmpTo(me.pos.x+dx[k], me.pos.y+dy[k]);
            if(coreData->mapMem(tmpTo).findElement(HARDWALL)
                || coreData->mapMem(tmpTo).findElement(SOFTWALL)
                || coreData->mapMem(tmpTo).findElement(BOMB)
                || coreData->mapMem(tmpTo).findElement(PLAYER)
                || (!coreData->mapMem(tmpTo).movingtoEntities.empty() && !(coreData->mapMem(tmpTo).movingtoEntities[0] == me.pos)))
                continue;
            target = tmpTo;
            me.posTo = tmpTo;
            me.isMoving = true;
            if (!isthereWave(me.pos, coreData) && isthereWave(tmpTo, coreData)) return NE;
            return me.direction();
        }
    }
    for (int i = 1; i <= 5; i++) {
        for (int j = 0; j < 4; j++){
            loca now(me.pos.x + i*dx[j], me.pos.y + i*dy[j]);
            if(now.x < 0 || now.x >= coreData->szN || now.y < 0 || now.y >= coreData->szM) continue;
            if(!~map[now.x][now.y]) continue;
            QList<QVariant>::Iterator it;
            auto &nowMem = coreData->mapMem(now);
            findo(it, BOMB, nowMem){
                if(it->toMap()["level"].toInt() >= i){
                    mode = FLEE;
                    if (j < 2){
                        for(int k = 2; k < 4; k++){
                            loca tmpTo(me.pos.x+dx[k], me.pos.y+dy[k]);
                            if (coreData->mapMem(tmpTo).findElement(HARDWALL)
                                || coreData->mapMem(tmpTo).findElement(SOFTWALL)
                                || coreData->mapMem(tmpTo).findElement(BOMB)
                                || coreData->mapMem(tmpTo).findElement(PLAYER)
                                || (!coreData->mapMem(tmpTo).movingtoEntities.empty() && !(coreData->mapMem(tmpTo).movingtoEntities[0] == me.pos))
                                    ) continue;
                            if (me.isMoving && me.posTo != tmpTo)
                                me.stop(coreData);
                            target = tmpTo;
                            me.posTo = tmpTo;
                            me.isMoving = true;
                            if (!isthereWave(me.pos, coreData) && isthereWave(tmpTo, coreData)) return NE;
                            return me.direction();
                        }
                        target = loca(me.pos.x+dx[j^1], me.pos.y+dy[j^1]);
                        return DD[j^1];
                    }
                    else{
                        for(int k = 0; k < 2; k++){
                            loca tmpTo(me.pos.x+dx[k], me.pos.y+dy[k]);
                            if(coreData->mapMem(tmpTo).findElement(HARDWALL)
                                || coreData->mapMem(tmpTo).findElement(SOFTWALL)
                                || coreData->mapMem(tmpTo).findElement(BOMB)
                                || coreData->mapMem(tmpTo).findElement(PLAYER)
                               || (!coreData->mapMem(tmpTo).movingtoEntities.empty() && !(coreData->mapMem(tmpTo).movingtoEntities[0] == me.pos))
                                    ) continue;
                            if (me.isMoving && me.posTo != tmpTo)
                                me.stop(coreData);
                            target = tmpTo;
                            me.posTo = tmpTo;
                            me.isMoving = true;
                            if (!isthereWave(me.pos, coreData) && isthereWave(tmpTo, coreData)) return NE;
                            return me.direction();
                        }
                        target = loca(me.pos.x+dx[j^1], me.pos.y+dy[j^1]);
                        return DD[j^1];
                    }
                }
            }
        }
    }
    if (mode == ATK && coreData->isDie(targetPlayerNum)) mode = NONE;
    if (mode == OPEN && !coreData->mapMem(target).findElement(SOFTWALL)) mode = NONE;
    if (mode == EAT && !coreData->mapMem(target).findElement(ITEM)) mode = NONE;
begin:
    loca movingto;
    switch (mode){
    case ATK:
        target = pre[coreData->players[targetPlayerNum].x][coreData->players[targetPlayerNum].y];
    case OPEN:
    case FLEE:
    case EAT:
        if (!~map[target.x][target.y]){
            mode = NONE;
            goto begin;
        }

        if (target == me.pos){
            if (mode == FLEE || mode == EAT){
                mode = NONE;
                goto begin;
            }
            mode = NONE;
            return DirectionType::BNB;
        }
        movingto = target;
        while (!(pre[movingto.x][movingto.y] == me.pos))
            movingto = pre[movingto.x][movingto.y];
        if (me.isMoving && !(me.posTo == movingto))
            me.stop(coreData);
        me.posTo = movingto;
        me.isMoving = true;
        return isthereWave(me.posTo, coreData) ? NE : me.direction();
        break;
    case NONE:
        //find a man to attack
        for (int i = 0; i < coreData->players.size(); i++){
            if (coreData->isDie(i) || map[coreData->players[i].x][coreData->players[i].y] == -1) continue;
            if (i == myNum) continue;
            mode = ATK;
            targetPlayerNum = i;
            goto begin;
        }
        //find a item to eat
        for (int i = 0; i < coreData->szN; i++)
            for (int j = 0; j < coreData -> szM; j++){
                if (map[i][j] == -1) continue;
                if (coreData->mapMem(i, j).findElement(ITEM) && !coreData->mapMem(i, j).findElement(SOFTWALL)){
                     mode = EAT;
                     target = loca(i, j);
                     goto begin;
                }
            }
        //find a softwall to open
        for (int i = 0; i < coreData->szN; i++)
            for (int j = 0; j < coreData -> szM; j++){
                if (map[i][j] == -1) continue;
                if (coreData->mapMem(i, j).findElement(SOFTWALL)){
                     mode = OPEN;
                     target = pre[i][j];
                     goto begin;
                }
            }
    }
    return DirectionType::NE;
}

bool Ai::isthereWave(loca pos, GameMainMap *coreData){
    Player tmpme(pos, 1, "");
    if (coreData->mapMem(tmpme.pos).findElement(BOMB) || coreData->mapMem(tmpme.pos).findElement(WAVE))
        return true;
    for (int i = 1; i <= 5; i++)
        for (int j = 0; j < 4; j++) {
            loca now(tmpme.pos.x + i*dx[j], tmpme.pos.y + i*dy[j]);
            QList<QVariant>::Iterator it;
            if(now.x < 0 || now.x >= coreData->szN || now.y < 0 || now.y >= coreData->szM) continue;
            if(!~map[now.x][now.y]) continue;
            auto &nowMem = coreData->mapMem(now);
            findo(it, BOMB, nowMem)
                if(it->toMap()["level"].toInt() >= i)
                        return true;
        }
    return false;
}
