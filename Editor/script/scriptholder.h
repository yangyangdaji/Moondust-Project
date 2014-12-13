/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * The script holder holds "commands" which can be later compiled to "autocode", "lunalua", "pgelua".
 * The compiler class just wraps the basic functions for the programming language and puts the segments
 * generated by the command class together.
 *
 */

#ifndef SCRIPTHOLDER_H
#define SCRIPTHOLDER_H

#include <QObject>

#include "command_compiler/basiccompiler.h"
#include "commands/basiccommand.h"
#include "commands/eventcommand.h"

class ScriptHolder : public QObject
{
    Q_OBJECT
public:
    explicit ScriptHolder(QObject *parent = 0);
    ~ScriptHolder();

    BasicCompiler *usingCompiler() const;
    void setUsingCompiler(BasicCompiler *usingCompiler);
    Script::CompilerType usingCompilerType() const;

    QString compileCode();

    QList<EventCommand *> events() const;
    void setEvents(const QList<EventCommand *> &events);

    QList<EventCommand *> &revents();
    EventCommand* findEvent(EventCommand::EventType evType);
    bool deleteEvent(EventCommand* evCmd);


signals:

public slots:

private:
    BasicCompiler* m_usingCompiler;
    QList<EventCommand *> m_events;
};

#endif // SCRIPTHOLDER_H
