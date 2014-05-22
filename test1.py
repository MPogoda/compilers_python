class someclass:
    def foo( self, x ):
        if x == 5:
            print( "foo" )
        else:
            print( "bar" )

    def bar( self, a, b ):
        c = b - a
        return c

class someotherclass:
    def foo( self, a, b):
        c = someclass()
        z = c.bar( a, b )
        c.foo( z )

class main:
    def run( self ):
        m = someotherclass()
        m.foo( 5, 10 )
        m.foo( 3, 6 )
        a = 1.53
        c = "asdd{}fgkjhns sdf asd "
        d = False

main().run()
