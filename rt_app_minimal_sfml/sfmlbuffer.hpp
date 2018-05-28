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

		SFMLBuffer(int x, int y, int threadCount = 1) : Buffer(x, y, threadCount)
		{
		}

		void Copy()
		{
			WriteLock();
			m_SFMLImage.create(mSizex, mSizey, (unsigned char*)mData);
			WriteUnlock();
			m_SFMLTexture.loadFromImage(m_SFMLImage);
			m_SFMLSprite.setTexture(m_SFMLTexture);
		}
	};
}
