import asyncio
import logging
from aiohttp import web
import aiohttp_cors

import sqlalchemy
import databases

import sqlalchemy_utils

import os

# Synchronous
ENGINE = None
CONNECTION = None

# Asynchronous
DIR_PATH = ''
DATABASE = None

TODOS = {
    0: {'title': 'build an API', 'order': 1, 'completed': False},
    1: {'title': '?????', 'order': 2, 'completed': False},
    2: {'title': 'profit!', 'order': 3, 'completed': False}
}

TAGS = {
    0: {'title': 'a common tag'},
}

MAP = {
    0: {'todo_id': 1, 'tag_id': 0}
}

METADATA = sqlalchemy.MetaData()

####### Initialize Tables #######
TODOS_TABLE = sqlalchemy.Table('TODOS', METADATA,
                               sqlalchemy.Column('id', sqlalchemy.Integer, primary_key=True),
                               sqlalchemy.Column('title', sqlalchemy.String),
                               sqlalchemy.Column('order', sqlalchemy.Integer),
                               sqlalchemy.Column('completed', sqlalchemy.Boolean),
                               sqlalchemy.Column('url', sqlalchemy.String))

TAGS_TABLE = sqlalchemy.Table('TAGS', METADATA,
                              sqlalchemy.Column('id', sqlalchemy.Integer, primary_key=True),
                              sqlalchemy.Column('title', sqlalchemy.String),
                              sqlalchemy.Column('url', sqlalchemy.String))

RELATION_TABLE = sqlalchemy.Table('MAP', METADATA, sqlalchemy.Column('id', sqlalchemy.Integer, primary_key=True),
                                  sqlalchemy.Column('todo_id', sqlalchemy.Integer),
                                  sqlalchemy.Column('tag_id', sqlalchemy.Integer))


############################################################################


####### TODOS Database Functions #######

async def todo_database_insert(identifier, title, order, completed, url):
    try:
        query = TODOS_TABLE.insert().values(id=identifier, title=title, order=order, completed=completed, url=url)
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not create todo with id: ', identifier, ' title: ', title, ' order: ', order, ' completed:',
              completed, ' url:', url)
        print(e)


async def todo_database_select(identifier):
    try:
        query = TODOS_TABLE.select().where(TODOS_TABLE.c.id == identifier)
        result = await DATABASE.fetch_one(query=query)
        return result
    except Exception as e:
        print('ERROR: Could not get todo with id:', identifier)
        print(e)


async def todo_database_select_all():
    try:
        query = TODOS_TABLE.select()
        result = await DATABASE.fetch_all(query=query)
        return result
    except Exception as e:
        print('ERROR: Could not get all todos from database')
        print(e)


async def todo_database_update(identifier, title=None, order=None, completed=None, url=None):
    try:
        if title:
            query = TODOS_TABLE.update().where(TODOS_TABLE.c.id == identifier).values(title=title)
            await DATABASE.execute(query=query)
        if order:
            query = TODOS_TABLE.update().where(TODOS_TABLE.c.id == identifier).values(order=order)
            await DATABASE.execute(query=query)
        if completed:
            query = TODOS_TABLE.update().where(TODOS_TABLE.c.id == identifier).values(completed=completed)
            await DATABASE.execute(query=query)
        if url:
            query = TODOS_TABLE.update().where(TODOS_TABLE.c.id == identifier).values(url=url)
            await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not update todo with id: ', identifier, ' title: ', title, ' order:', order, ' completed:',
              completed, ' url:', url)
        print(e)


async def todo_database_delete(identifier):
    try:
        query = TODOS_TABLE.delete().where(TODOS_TABLE.c.id == identifier)
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not remove todo with id ', identifier, ' from database')
        print(e)


async def todo_database_delete_all():
    try:
        query = TODOS_TABLE.delete()
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not remove all todos from database')
        print(e)


async def todo_database_todo_exists(identifier):
    try:
        query = TODOS_TABLE.select().where(TODOS_TABLE.c.id == identifier)
        result = await DATABASE.fetch_all(query=query)
        for row in result:
            return True
        return False

    except Exception as e:
        print('ERROR: Could not check if todo with id:', identifier, ' exists in database')
        print(e)


async def todo_database_count():
    try:
        count = 0
        query = TODOS_TABLE.select()
        result = await DATABASE.fetch_all(query=query)
        for row in result:
            count = max(row[0], count)
        return count
    except Exception as e:
        print('Could not get the count of entries in table database')
        print(e)


############################################################################


####### TAGS Database Functions ########

async def tag_database_insert(identifier, title, url):
    try:
        query = TAGS_TABLE.insert().values(id=identifier, title=title, url=url)
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not create tag with id: ', identifier, ' title: ', title, ' url:', url)
        print(e)


async def tag_database_update(identifier, title=None, url=None):
    try:
        if title:
            query = TAGS_TABLE.update().where(TAGS_TABLE.c.id == identifier).values(title=title)
            await DATABASE.execute(query=query)
        if url:
            query = TAGS_TABLE.update().where(TAGS_TABLE.c.id == identifier).values(url=url)
            await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not update tag with id: ', identifier)
        print(e)


async def tag_database_select(identifier):
    try:
        query = TAGS_TABLE.select().where(TAGS_TABLE.c.id == identifier)
        result = await DATABASE.fetch_one(query=query)
        return result
    except Exception as e:
        print('ERROR: Could not get tag with id:', identifier)
        print(e)


async def tag_database_select_all():
    try:
        query = TAGS_TABLE.select()
        result = await DATABASE.fetch_all(query=query)
        return result
    except Exception as e:
        print('ERROR: Could not get all tags from database')
        print(e)


async def tag_database_delete(identifier):
    try:
        query = TAGS_TABLE.delete().where(TAGS_TABLE.c.id == identifier)
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not remove tag with id ', identifier, ' from database')
        print(e)


async def tag_database_delete_all():
    try:
        query = TAGS_TABLE.delete()
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not remove all tags from database')
        print(e)


async def tag_database_tag_exists(identifier):
    try:
        query = TAGS_TABLE.select().where(TAGS_TABLE.c.id == identifier)
        result = await DATABASE.fetch_all(query=query)
        for row in result:
            return True
        return False
    except Exception as e:
        print('ERROR: Could not check if tag with id:', identifier, ' exists in database')
        print(e)


async def tag_database_count():
    try:
        count = 0
        query = TAGS_TABLE.select()
        result = await DATABASE.fetch_all(query=query)
        for row in result:
            count = max(row[0], count)
        return count
    except Exception as e:
        print('Could not get the count of entries in table database')
        print(e)


############################################################################


####### RELATION Database Functions #######

async def relation_database_insert(todo_identifier, tag_identifier):
    try:
        query = RELATION_TABLE.insert().values(todo_id=todo_identifier, tag_id=tag_identifier)
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not create relation entry with tag_id: ', tag_identifier, ' and todo_id: ', todo_identifier)
        print(e)


async def relation_database_select(todo_identifier, tag_identifier):
    try:
        query = RELATION_TABLE.select().where(RELATION_TABLE.c.todo_id == todo_identifier).where(
            RELATION_TABLE.c.tag_id == tag_identifier)
        result = await DATABASE.fetch_all(query=query)
        return result
    except Exception as e:
        print('ERROR: Could not get relation entries with tag_id:', tag_identifier, ' and todo_id: ', todo_identifier)
        print(e)


async def relation_database_select_todo(todo_identifier):
    try:
        query = RELATION_TABLE.select().where(RELATION_TABLE.c.todo_id == todo_identifier)
        result = await DATABASE.fetch_all(query=query)
        return result
    except Exception as e:
        print('ERROR: Could not get relation entries with todo_id:', id)
        print(e)


async def relation_database_select_tag(tag_identifier):
    try:
        query = RELATION_TABLE.select().where(RELATION_TABLE.c.tag_id == tag_identifier)
        result = await DATABASE.fetch_all(query=query)
        return result
    except Exception as e:
        print('ERROR: Could not get relation entries with tag_id:', id)
        print(e)


async def relation_database_select_all():
    try:
        query = RELATION_TABLE.select()
        result = await DATABASE.fetch_all(query=query)
        return result
    except Exception as e:
        print('ERROR: Could not get all relation entries')
        print(e)


async def relation_database_remove(todo_identifier, tag_identifier):
    try:
        query = RELATION_TABLE.select().where(RELATION_TABLE.c.todo_id == todo_identifier).where(
            RELATION_TABLE.c.tag_id == tag_identifier)
        await DATABASE.execute(query=query)

    except Exception as e:
        print('ERROR: Could not remove relation entries with tag_id: ', tag_identifier, ' and todo_id:', todo_identifier,
              ' from database')
        print(e)


async def relation_database_remove_todo_association(todo_identifier):
    try:
        query = RELATION_TABLE.delete().where(RELATION_TABLE.c.todo_id == todo_identifier)
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not remove relation entries with todo_id: ', todo_identifier, ' from database')
        print(e)


async def relation_database_remove_tag_association(tag_identifier):
    try:
        query = RELATION_TABLE.delete().where(RELATION_TABLE.c.tag_id == tag_identifier)
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not remove relation entries with tag_id: ', tag_identifier, ' from database')
        print(e)


async def relation_database_remove_all():
    try:
        query = RELATION_TABLE.delete()
        await DATABASE.execute(query=query)
    except Exception as e:
        print('ERROR: Could not remove all relation entries from database')
        print(e)


async def relation_database_count():
    try:
        count = 0
        query = RELATION_TABLE.select()
        result = await DATABASE.fetch_all(query=query)
        for row in result:
            count = count + 1
        return count
    except Exception as e:
        print('Could not get the count of entries in relation database')
        print(e)


############################################################################


####### Main Functions #######

### TODOS Main Functions ###

async def create_todo(request):
    data = await request.json()

    order = None

    if 'title' not in data:
        return web.json_response({'error': '"title" is a required field'})

    title = data['title']

    if not isinstance(title, str) or not len(title):
        return web.json_response({'error': '"title" must be a string with at least one character'})

    data['completed'] = bool(data.get('completed', False))

    if 'order' not in data:
        order = 0
    else:
        order = data['order']

    new_id = await todo_database_count() + 1

    data['url'] = str(request.url.join(request.app.router['one_todo'].url_for(id=str(new_id))))

    await todo_database_insert(new_id, title, order, data['completed'], data['url'])

    return web.Response(
        headers={'Location': data['url']},
        status=303
    )


async def update_todo(request):
    identifier = int(request.match_info['id'])
    data = await request.json()

    order = None
    completed = None
    title = None
    url = None

    if not await todo_database_todo_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    if 'order' in data:
        order = data['order']
    if 'title' in data:
        title = data['title']
    if 'completed' in data:
        completed = data['completed']
    if 'url' in data:
        url = data['url']

    await todo_database_update(identifier, title, order, completed, url)

    todo = await todo_database_select(identifier)

    return web.json_response(
        {'id': todo[0], 'title': todo[1], 'order': todo[2], 'completed': todo[3],
         'url': todo[4]})


async def get_one_todo(request):
    identifier = int(request.match_info['id'])
    if not await todo_database_todo_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    todo = await todo_database_select(identifier)
    tags = []
    database_relation = await relation_database_select_all()
    for db_relation in database_relation:
        if db_relation[1] == identifier:
            tag = await tag_database_select(db_relation[2])
            tags.append({'id': tag[0], 'title': tag[1], 'url': tag[2]})

    return web.json_response(
        {'id': todo[0], 'title': todo[1], 'order': todo[2], 'completed': todo[3],
         'url': todo[4], 'tags': tags})


async def get_all_todos(request):
    await DATABASE.connect()
    todos = []
    database_todos = await todo_database_select_all()
    database_relation = await relation_database_select_all()
    for db_todo in database_todos:
        tags = []
        for db_relation in database_relation:
            if db_relation[1] == db_todo[0]:
                tag = await tag_database_select(db_relation[2])
                tags.append({'id': tag[0], 'title': tag[1], 'url': tag[2]})

        todos.append({'id': db_todo[0], 'title': db_todo[1], 'order': db_todo[2], 'completed': db_todo[3],
                      'url': db_todo[4], 'tags': tags})

    await DATABASE.disconnect()
    return web.json_response(todos)


async def remove_todo(request):
    identifier = int(request.match_info['id'])
    await DATABASE.connect()

    if not await todo_database_todo_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    await relation_database_remove_todo_association(identifier)

    await todo_database_delete(identifier)

    await DATABASE.disconnect()
    return web.Response(status=204)


async def remove_all_todos(request):
    await todo_database_delete_all()
    await relation_database_remove_all()

    return web.Response(status=204)


############################################################################


### Tag Main Functions ###

async def create_tag(request):
    data = await request.json()

    if 'title' not in data:
        return web.json_response({'error': '"title" is a required field'})

    title = data['title']

    if not isinstance(title, str) or not len(title):
        return web.json_response({'error': '"title" must be a string with at least one character'})

    new_id = await tag_database_count() + 1

    data['url'] = str(request.url.join(request.app.router['one_tag'].url_for(id=str(new_id))))

    await tag_database_insert(new_id, title, data['url'])

    return web.Response(
        headers={'Location': data['url']},
        status=303
    )


async def update_tag(request):
    identifier = int(request.match_info['id'])
    data = await request.json()
    title = None
    url = None

    if not await tag_database_tag_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    if 'title' in data:
        title = data['title']
    if 'url' in data:
        url = data['url']

    await tag_database_update(identifier, title, url)

    tag = await tag_database_select(identifier)

    return web.json_response({'id': tag[0], 'title': tag[1], 'url': tag[2]})


async def get_all_tags(request):
    await DATABASE.connect()
    tags = []
    database_tags = await tag_database_select_all()
    database_relation = await relation_database_select_all()

    for db_tag in database_tags:
        todos = []
        for db_relation in database_relation:
            if db_relation[2] == db_tag[0]:
                todo = await todo_database_select(db_relation[1])
                todos.append({'id': todo[0], 'title': todo[1], 'order': todo[2], 'completed': todo[3], 'url': todo[4]})

        tags.append({'id': db_tag[0], 'title': db_tag[1], 'url': db_tag[2], 'todos': todos})

    await DATABASE.disconnect()

    return web.json_response(tags)


async def get_one_tag(request):
    identifier = int(request.match_info['id'])

    if not await tag_database_tag_exists(identifier):
        return web.json_response({'error': 'Tag not found'}, status=404)

    tag = await tag_database_select(identifier)
    database_relation = await relation_database_select_all()
    todos = []
    for db_relation in database_relation:
        if db_relation[2] == tag[0]:
            todo = await todo_database_select(db_relation[1])
            todos.append({'id': todo[0], 'title': todo[1], 'order': todo[2], 'completed': todo[3], 'url': todo[4]})

    return web.json_response({'id': tag[0], 'title': tag[1], 'url': tag[2], 'todos': todos})


async def get_todos_of_tag(request):
    identifier = int(request.match_info['id'])

    if not await tag_database_tag_exists(identifier):
        return web.json_response({'error': 'Tag not found'}, status=404)

    tag = await tag_database_select(identifier)
    database_relation = await relation_database_select_all()
    todos = []
    for db_relation in database_relation:
        if db_relation[2] == tag[0]:
            todo = await todo_database_select(db_relation[1])
            todos.append({'id': todo[0], 'title': todo[1], 'order': todo[2], 'completed': todo[3], 'url': todo[4]})

    return web.json_response(todos)


async def get_tags_from_todo(request):
    identifier = int(request.match_info['id'])

    if not await todo_database_todo_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    database_relation = await relation_database_select_all()
    tags = []
    for db_relation in database_relation:
        if db_relation[1] == identifier:
            tag = await tag_database_select(db_relation[2])
            tags.append({'id': tag[0], 'title': tag[1], 'url': tag[2]})

    return web.json_response(tags)


async def associate_tag_to_todo(request):
    data = await request.json()

    identifier = int(request.match_info['todo_id'])

    if 'id' not in data:
        return web.json_response({'error': '"id" is a required field'})

    if not await todo_database_todo_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    tag_id = data['id']

    await relation_database_insert(identifier, tag_id)  # reversed
    return web.Response(status=201)


async def remove_all_tags(request):
    await tag_database_delete_all()
    await relation_database_remove_all()

    return web.Response(status=204)


async def remove_tag(request):
    identifier = int(request.match_info['id'])
    await DATABASE.connect()

    if not await tag_database_tag_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    await relation_database_remove_tag_association(identifier)

    await tag_database_delete(identifier)

    await DATABASE.disconnect()
    return web.Response(status=204)


async def remove_tag_from_todo(request):
    identifier = int(request.match_info['id'])
    tag_id = int(request.match_info['tag_id'])

    if not await todo_database_todo_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    if not await tag_database_tag_exists(tag_id):
        return web.json_response({'error': 'Tag not found'}, status=404)

    await relation_database_remove(identifier, tag_id)
    return web.Response(status=204)


async def remove_tags_from_todo(request):
    identifier = int(request.match_info['id'])

    if not await todo_database_todo_exists(identifier):
        return web.json_response({'error': 'Todo not found'}, status=404)

    await relation_database_remove_todo_association(identifier)
    return web.Response(status=204)


############################################################################


app = web.Application()

# Configure Database Engine
DIR_PATH = os.path.dirname(os.path.realpath(__file__))

ENGINE = sqlalchemy.create_engine('sqlite:///' + DIR_PATH + '\\app_database.db')
if not sqlalchemy_utils.database_exists('sqlite:///' + DIR_PATH + '\\app_database.db'):
    print('No database found. Creating new one.')

    sqlalchemy_utils.create_database('sqlite:///' + DIR_PATH + '\\app_database.db')
    try:
        METADATA.create_all(ENGINE)
    except Exception as e:
        print('Error during table creation.')
        print(e)

CONNECTION = ENGINE.connect()

DATABASE = databases.Database('sqlite:///' + DIR_PATH + '\\app_database.db')

# Configure default CORS settings.
cors = aiohttp_cors.setup(app, defaults={
    "*": aiohttp_cors.ResourceOptions(
        allow_credentials=True,
        expose_headers="*",
        allow_headers="*",
        allow_methods="*",
    )
})

cors.add(app.router.add_get('/todos/', get_all_todos, name='all_todos'))
cors.add(app.router.add_delete('/todos/', remove_all_todos, name='remove_todos'))
cors.add(app.router.add_post('/todos/', create_todo, name='create_todo'))
cors.add(app.router.add_get('/todos/{id:\d+}', get_one_todo, name='one_todo'))
cors.add(app.router.add_patch('/todos/{id:\d+}', update_todo, name='update_todo'))
cors.add(app.router.add_delete('/todos/{id:\d+}', remove_todo, name='remove_todo'))

####### Additional functions ########
# Tag-association with todos
cors.add(app.router.add_delete('/todos/{id:\d+}/tags/', remove_tags_from_todo, name='remove_tags_from_todo'))
cors.add(app.router.add_get('/todos/{id:\d+}/tags/', get_tags_from_todo, name='get_tags_from_todo'))
cors.add(app.router.add_post('/todos/{todo_id:\d+}/tags/', associate_tag_to_todo, name='associate_tag_to_todo'))
cors.add(app.router.add_delete('/todos/{id:\d+}/tags/{tag_id:\d+}', remove_tag_from_todo, name='remove_tag_from_todo'))
# tag functions
cors.add(app.router.add_get('/tags/', get_all_tags, name='all_tags'))
cors.add(app.router.add_delete('/tags/', remove_all_tags, name='remove_tags'))
cors.add(app.router.add_post('/tags/', create_tag, name='create_tag'))
cors.add(app.router.add_delete('/tags/{id:\d+}', remove_tag, name='remove_tag'))
cors.add(app.router.add_get('/tags/{id:\d+}', get_one_tag, name='one_tag'))
cors.add(app.router.add_patch('/tags/{id:\d+}', update_tag, name='update_tag'))
cors.add(app.router.add_get('/tags/{id:\d+}/todos/', get_todos_of_tag, name='todos_of_tag'))

logging.basicConfig(level=logging.DEBUG)
web.run_app(app, port=8080)
