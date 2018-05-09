#include <rt/buffer/buffer.hpp>

#include <SFML/Graphics.hpp>

namespace rt
{
	class SFMLBuffer : public Buffer
	{
	public:
		sf::Image m_SFMLImage;
		sf::Texture m_SFMLTexture;
		sf::Sprite m_SFMLSprite;

		SFMLBuffer(int x, int y) : Buffer(x, y)
		{
		}

		void Copy()
		{
			m_SFMLImage.create(mSizex, mSizey, (unsigned char*)mData);
			m_SFMLTexture.loadFromImage(m_SFMLImage);
			m_SFMLSprite.setTexture(m_SFMLTexture);
		}
	};
}
